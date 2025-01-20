// Property of Paracosm Industries. Dont use my shit.


#include "Character/TimberPlayableCharacter.h"
#include "BuildSystem/BuildingComponents/TimberBuildingComponentBase.h"
#include "BuildSystem/Ramps/RampBase.h"
#include "BuildSystem/Traps/TrapBase.h"
#include "Components/BuildSystem/BuildSystemManagerComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/Inventory/InventoryManagerComponent.h"
#include "Controller/TimberPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameModes/TimberGameModeBase.h"
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

	/*Delegate Binding*/
	Cast<ATimberHUDBase>(Cast<ATimberPlayerController>(GetController())->GetHUD())->bIsBuildMenuOpen.AddDynamic(
		this,
		&ATimberPlayableCharacter::HandleBuildMenuOpen);

	RaycastController = Cast<ATimberPlayerController>(GetController());
	
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

			/* Single Hit*/
			/*FHitResult HitResult;
			bool bHit = GetWorld()->LineTraceSingleByChannel(
				HitResult,
				RaycastStart,
				RaycastEnd,
				ECC_Visibility,
				CollisionParams);*/

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
		//TODO:: This can be reduced and simplified 
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
			UE_LOG(LogTemp, Warning, TEXT("TimberPlayableCharacter.cpp - Hovered Building Component: %s"), *HoveredBuildingComponent->GetName());
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
	
	
	/*AActor* HitActor = HitResults.GetComponent()->GetOwner();
	//Hit can be one of the quadrants so we want to make sure that the hit is a Building Component
	if (Cast<ATimberBuildingComponentBase>(HitActor))
	{
		HoveredBuildingComponent = Cast<ATimberBuildingComponentBase>(HitActor);
		if (HoveredBuildingComponent)
		{
			//Translating a Location in World Space to Its Location in Screen Space
			FVector2d ScreenLocationOfImpactPoint;
			GetWorld()->GetFirstPlayerController()->ProjectWorldLocationToScreen(
				HitResults.ImpactPoint,
				ScreenLocationOfImpactPoint);

			// Broadcast a Delegate with the Impact Position to the HUD.
			HandleSpawnDeleteIconLocation_DelegateHandle.Broadcast(
				ScreenLocationOfImpactPoint.X, ScreenLocationOfImpactPoint.Y);
		}

		return true;
	}
	if (Cast<ATimberBuildingComponentBase>(HitResults.GetActor()))
	{
		HoveredBuildingComponent = Cast<ATimberBuildingComponentBase>(HitResults.GetActor());
		if (HoveredBuildingComponent)
		{
			//Translating a Location in World Space to Its Location in Screen Space
			FVector2d ScreenLocationOfImpactPoint;
			GetWorld()->GetFirstPlayerController()->ProjectWorldLocationToScreen(
				HitResults.ImpactPoint,
				ScreenLocationOfImpactPoint);

			// Broadcast a Delegate with the Impact Position to the HUD.
			HandleSpawnDeleteIconLocation_DelegateHandle.Broadcast(
				ScreenLocationOfImpactPoint.X, ScreenLocationOfImpactPoint.Y);
		}
	}*/
	


	return false;
}

void ATimberPlayableCharacter::HandleBuildMenuOpen(bool bIsBuildMenuOpen)
{
	ShouldRaycast = !bIsBuildMenuOpen;
}

/*Death & Damage*/

void ATimberPlayableCharacter::PlayDeathAnimation()
{
	PlayAnimMontage(DeathMontage, 1.f, FName("Death1"));
}

void ATimberPlayableCharacter::HandlePlayerDeath()
{
	if (bIsPlayerDead)
	{
		PlayDeathAnimation();
		//Broadcasting the Player Death Delegate
		//Player Controller is Subscribed to this Delegate
		HandlePlayerDeath_DelegateHandle.Broadcast(bIsPlayerDead);
	}
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
		HandlePlayerDeath();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Hit for: %f. CurrentHealth: %f."), DamageAmount, CurrentHealth);
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
