// Property of Paracosm Industries. Dont use my shit.


#include "Character/TimberPlayableCharacter.h"
#include "BuildSystem/BuildingComponents/TimberBuildingComponentBase.h"
#include "BuildSystem/Ramps/RampBase.h"
#include "BuildSystem/Traps/TrapBase.h"
#include "Components/BuildSystem/BuildSystemManagerComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/TimberSeeda.h"
#include "Components/Inventory/InventoryManagerComponent.h"
#include "Controller/TimberPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameModes/TimberGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/TimberHUDBase.h"

ATimberPlayableCharacter::ATimberPlayableCharacter()
{
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Camera->SetupAttachment(CameraSpringArm);
	CameraSpringArm->SetupAttachment(RootComponent);

	/* Actor Components */
	BuildSystemManager = CreateDefaultSubobject<UBuildSystemManagerComponent>("BuildSystemManager");
	InventoryManager = CreateDefaultSubobject<UInventoryManagerComponent>("InventoryManager");
}

void ATimberPlayableCharacter::BeginPlay()
{
	Super::BeginPlay();

	/* Load Inventory */
	GetPlayerInventoryFromPlayerState();

	/* Set Character Movement Defaults*/
	GetCharacterMovement()->MaxWalkSpeed = 800.f;

	RaycastController = Cast<ATimberPlayerController>(GetController());

	/*Delegate Binding*/
	Cast<ATimberHUDBase>(Cast<ATimberPlayerController>(GetController())->GetHUD())->bIsBuildMenuOpen.AddDynamic(
		this,
		&ATimberPlayableCharacter::HandleBuildMenuOpen);


	{
		//Binding to the GameMode Delegate to know when the Seeda is Initialized and then Binding to the Seeda Delegates
		ATimberGameModeBase* GM = Cast<ATimberGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			UE_LOG(LogTemp, Warning, TEXT("Binding to GameMode Delegate to know when Seeda is Initialized."));
			//Basically says now you can Bind to the Seeda Delegates, and Passes in the Seeda Actor Ref.
			//This is possible because we know for sure that the GM instance is available for binding by the time this BeginPlay fires.
			GM->OnSeedaSpawn.AddDynamic(this, &ATimberPlayableCharacter::BindToSeedaDelegates);
		}
	}
	
	{
		 /*
		  *Broadcasts a Delegate on the GameMode letting other Systems know that the Character is Initialized.
		  *Allows other systems to bind to the Game Mode Delegate which is typically initialized before anything else.
		  *Used to eliminate potential Initialization Races
		  *Set at the bottoms of the Begin Play Function to ensure all other systems are initialized.
		  */
		ATimberGameModeBase* GM = Cast<ATimberGameModeBase>(GetWorld()->GetAuthGameMode());
		if(GM)
		{
			GM->PlayerIsInitialized();
		}
	}
}

/*Delegate Stuff*/

void ATimberPlayableCharacter::BindToSeedaDelegates(AActor* Seeda)
{
	ATimberSeeda* SeedaRef = Cast<ATimberSeeda>(Seeda);
	if (SeedaRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Seeda Initialized, Binding to Seeda Delegates."));

		//When seeda dies, handle player death
		SeedaRef->OnSeedaDeath.AddDynamic(this, &ATimberPlayableCharacter::HandlePlayerDeath);
		//When Seeda is being Destroyed
		SeedaRef->OnDestroyed.AddDynamic(this, &ATimberPlayableCharacter::UnbindFromSeedaDeathDelegate);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Seeda Not Initialized, Cannot Bind to Seeda Delegates."));
	}
}

void ATimberPlayableCharacter::UnbindFromSeedaDeathDelegate(AActor* DestroyedActor)
{
	//If Seeda Exists
	if (Cast<ATimberSeeda>(UGameplayStatics::GetActorOfClass(this, ATimberSeeda::StaticClass())))
	{
		//Unbind from Seeda Death Delegate - Will need to Rebind on Seeda Respawn, this gets Called when Seeda Dies.
		Cast<ATimberSeeda>(UGameplayStatics::GetActorOfClass(this, ATimberSeeda::StaticClass()))->OnSeedaDeath.RemoveDynamic(
			this,
			&ATimberPlayableCharacter::HandlePlayerDeath);
	}
}

void ATimberPlayableCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//Initiated From Player Controller Input
	if (CharacterState == ECharacterState::Building && ShouldRaycast)
	{
		PerformBuildSystemRaycast();
	}

	//TODO:: This can be made more efficient by only calling this for like 2 seconds after last movement or rotation.
	if (CurrentWeaponState == EWeaponState::RangedEquipped)
	{
		RaycastController->PerformReticuleAlignment_Raycast();
	}
}

/*Build System Stuff*/

void ATimberPlayableCharacter::PerformBuildSystemRaycast()
{
	if (CharacterState == ECharacterState::Building)
	{
		if (RaycastController)
		{
			FVector RaycastStart;
			FRotator PlayerRotation;
			Controller->GetPlayerViewPoint(RaycastStart, PlayerRotation);

			//1000 is the range to perform the Raycast.
			FVector RaycastEnd = RaycastStart + (PlayerRotation.Vector() * BuildRaycastDistance);

			/* Ignore the Player Raycasting and the Weapon*/
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(this);
			CollisionParams.AddIgnoredActor(this->CurrentlyEquippedWeapon);

			/*Multiple Hits*/
			bool bHits = GetWorld()->LineTraceMultiByChannel(
				HitResults,
				RaycastStart,
				RaycastEnd,
				ECC_Visibility,
				CollisionParams);

			HandleShowDeleteWidget();
			HandleRaycastHitConditions(bHits);
		}
	}
}

void ATimberPlayableCharacter::HandleRaycastHitConditions(bool bHits)
{
	TSubclassOf<ABuildableBase> ActiveBuildableClass = BuildSystemManager->GetActiveBuildableClass();
	if (bHits && ActiveBuildableClass)
	{
		/* Hit Something/Anything */
		DrawDebugSphere(GetWorld(), HitResults[0].ImpactPoint, 10.f, 8, FColor::Red, false, 0.1f);

		if (ActiveBuildableClass->IsChildOf(ATrapBase::StaticClass()))
		{
			BuildSystemManager->HandleTrapPlacement(HitResults);
		}

		if (ActiveBuildableClass->IsChildOf(ARampBase::StaticClass()))
		{
			BuildSystemManager->HandleRampPlacement(HitResults);
		}

		if (ActiveBuildableClass->IsChildOf(ATimberBuildingComponentBase::StaticClass()))
		{
			// Spawning ActiveBuildingComponent if it doesn't exist or if its different then the ActiveBuildingComponentClass
			ATimberBuildingComponentBase* ActiveBuildingComponentProxy = BuildSystemManager->
				GetActiveBuildingComponent();
			if (ActiveBuildingComponentProxy == nullptr || ActiveBuildingComponentProxy->GetClass() !=
				BuildSystemManager->GetActiveBuildableClass())
			{
				BuildSystemManager->SpawnBuildingComponentProxy(HitResults[0].ImpactPoint, GetActorRotation());
			}

			BuildSystemManager->HandleBuildingComponentPlacement(HitResults);
		}
	}
	else
	{
		BuildSystemManager->ResetBuildableComponents(ATrapBase::StaticClass());
		BuildSystemManager->ResetBuildableComponents(ATimberBuildingComponentBase::StaticClass());
		BuildSystemManager->ResetBuildableComponents(ARampBase::StaticClass());
	}
}

bool ATimberPlayableCharacter::HandleShowDeleteWidget()
{
	//Hit Results Passed in as a Global Variable
	//First Hovered Building Component
	HoveredBuildingComponent = nullptr;
	for(FHitResult Hits : HitResults)
	{
		if(Cast<ABuildableBase>(Hits.GetActor()))
		{
			HoveredBuildingComponent = Cast<ABuildableBase>(Hits.GetActor());
			//UE_LOG(LogTemp, Warning, TEXT("TimberPlayableCharacter.cpp - Hovered Building Component: %s"), *HoveredBuildingComponent->GetName());
			FVector2d ScreenLocationOfImpactPoint;

			/*Setting Impact Point at Location to show Delete Widget in Screen Space*/
			GetWorld()->GetFirstPlayerController()->ProjectWorldLocationToScreen(
				Hits.ImpactPoint,
				ScreenLocationOfImpactPoint);

			// Broadcast a Delegate with the Impact Position to the HUD.
			HandleSpawnDeleteIconLocation_DelegateHandle.Broadcast(
				ScreenLocationOfImpactPoint.X, ScreenLocationOfImpactPoint.Y);
		}
		break;
	}
	if(HoveredBuildingComponent == nullptr)
	{
		ResetDeleteIcon();
		return false;
	}
	
	return false;
}

void ATimberPlayableCharacter::HandleBuildMenuOpen(bool bIsBuildMenuOpen)
{
	ShouldRaycast = !bIsBuildMenuOpen;
}

/*Death & Damage*/
void ATimberPlayableCharacter::HandlePlayerDeath(bool bIsPlayerDeadNow)
{
	bIsPlayerDead = bIsPlayerDeadNow;
	if (bIsPlayerDead)
	{
		PlayDeathAnimation();
		//Broadcasting the Player Death Delegate
		//Player Controller is Subscribed to this Delegate
		HandlePlayerDeath_DelegateHandle.Broadcast(bIsPlayerDead);
	}
}

void ATimberPlayableCharacter::PlayDeathAnimation()
{
	PlayAnimMontage(DeathMontage, 1.f, FName("Death1"));
}

void ATimberPlayableCharacter::GetPlayerInventoryFromPlayerState()
{
	InventoryObject = GetController()->GetPlayerState<APlayerStateBase>()->MainInventory;
	if(InventoryObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Inventory Loaded."));
	}
}

void ATimberPlayableCharacter::ResetDeleteIcon()
{
	HoveredBuildingComponent = nullptr;
	HandleRemoveDeleteIcon_DelegateHandle.Broadcast();
}

void ATimberPlayableCharacter::PlayerTakeDamage(float DamageAmount)
{
	CurrentHealth -= DamageAmount;
	if (CurrentHealth <= 0.f)
	{
		bIsPlayerDead = true;
		HandlePlayerDeath(bIsPlayerDead);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Player Hit for: %f. CurrentHealth: %f."), DamageAmount, CurrentHealth);
	}
}

/*Weapon Stuff*/

void ATimberPlayableCharacter::SetCurrentWeaponState(EWeaponState NewWeaponState)
{
	CurrentWeaponState = NewWeaponState;
}

void ATimberPlayableCharacter::SetCurrentlyEquippedWeapon(ATimberWeaponBase* Weapon)
{
	CurrentlyEquippedWeapon = Weapon;
}

