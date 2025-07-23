// Property of Paracosm Industries. 


#include "Character/TimberPlayableCharacter.h"
#include "Character/TimberSeeda.h"
#include "Controller/TimberPlayerController.h"
#include "BuildSystem/Traps/TrapBase.h"
#include "BuildSystem/BuildingComponents/TimberBuildingComponentBase.h"
#include "Components/BuildSystem/BuildSystemManagerComponent.h"
#include "Components/Inventory/InventoryManagerComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/TimberAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/Combat/CombatComponent.h"
#include "Components/Vignette/PlayerVignetteComponent.h"
#include "Containers/Ticker.h"
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
	CombatComponent = CreateDefaultSubobject<UCombatComponent>("CombatComponent");
	VignetteComponent = CreateDefaultSubobject<UPlayerVignetteComponent>("VignetteComponent");

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("DR_PlayerCharacterCapsule"));
	GetMesh()->SetCollisionProfileName(TEXT("DR_AestheticMeshOnly"));
}

void ATimberPlayableCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	/* Set Character Movement Defaults*/
	GetCharacterMovement()->MaxWalkSpeed = 800.f;

	PlayerController = Cast<ATimberPlayerController>(GetController());
	
	/* Load Inventory */
	GetPlayerInventoryFromPlayerState();

	/*Initialize Anim Instance*/
	UTimberAnimInstance* Anim = Cast<UTimberAnimInstance>(GetMesh()->GetAnimInstance());
	if (Anim)
	{
		Anim->OwningPawn = this;
		Anim->PlayerController = Cast<ATimberPlayerController>(GetController());
	}
	
	/*Delegate Binding*/
	if (Cast<ATimberHUDBase>(Cast<ATimberPlayerController>(GetController())->GetHUD()))
	{
		Cast<ATimberHUDBase>(Cast<ATimberPlayerController>(GetController())->GetHUD())->bIsBuildMenuOpen.AddDynamic(
			this,
			&ATimberPlayableCharacter::HandleBuildMenuOpen);
	}
	
	//Binding to the GameMode Delegate to know when the Seeda is Initialized and then Binding to the Seeda Delegates
	ATimberGameModeBase* GM = Cast<ATimberGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Binding to GameMode Delegate to know when Seeda is Initialized."));
		//Basically says now you can Bind to the Seeda Delegates, and Passes in the Seeda Actor Ref.
		//This is possible because we know for sure that the GM instance is available for binding by the time this BeginPlay fires.
		GM->OnSeedaSpawn.AddDynamic(this, &ATimberPlayableCharacter::BindToSeedaDelegates);

		/*
	 *Broadcasts a Delegate on the GameMode letting other Systems know that the Character is Initialized.
	 *Allows other systems to bind to the Game Mode Delegate which is typically initialized before anything else.
	 *Used to eliminate potential Initialization Races
	 *Set at the bottoms of the Begin Play Function to ensure all other systems are initialized.
	 */
		GM->PlayerIsInitialized(this);
	}
	
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		CombatComponent->SpawnMeleeWeapon();
		CombatComponent->SpawnRangedWeapon();
	});
	
	/* Tutorial */
	if (GetTutorialState() == ETutorialState::Wake1)
	{
		PlayWakeAnimationMontage();
	}
}

void ATimberPlayableCharacter::BindToSeedaDelegates(AActor* Seeda)
{
	ATimberSeeda* SeedaRef = Cast<ATimberSeeda>(Seeda);
	if (SeedaRef)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Seeda Initialized, Binding to Seeda Delegates."));

		//When seeda dies, player also dies.
		SeedaRef->OnSeedaDeath.AddDynamic(this, &ATimberPlayableCharacter::HandlePlayerDeath);
		
		//When Seeda is being Destroyed
		SeedaRef->OnDestroyed.AddDynamic(this, &ATimberPlayableCharacter::UnbindFromSeedaDeathDelegate);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Seeda Not Initialized, Cannot Bind to Seeda Delegates."));
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

	/*//TODO:: This can be made more efficient by only calling this for like 2 seconds after last movement or rotation.
	//Used for aligning Projectile Direction with center of screen
	if (CurrentWeaponState == EWeaponState::RangedEquipped)
	{
		RaycastController->PerformReticuleAlignment_Raycast();
	}*/
}

/*Build System Stuff*/

void ATimberPlayableCharacter::PerformBuildSystemRaycast()
{
	if (CharacterState == ECharacterState::Building)
	{
		if (PlayerController)
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
	//Active Buildable class set from UI Click in Build Panel Menu
	if (BuildSystemManager)
	{
		TSubclassOf<ABuildableBase> ActiveBuildableClass = BuildSystemManager->GetActiveBuildableClass();
		
		if (bHits && ActiveBuildableClass)
		{
			//DrawDebugSphere(GetWorld(), HitResults[0].ImpactPoint, 10.f, 8, FColor::Red, false, 0.1f);
			BuildSystemManager->HandleProxyPlacement(HitResults, ActiveBuildableClass);
		}
		else //If the Raycast Hit Nothing
		{
			BuildSystemManager->ResetBuildableComponents();
		}
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
			/*UE_LOG(LogTemp, Warning, TEXT("TimberPlayableCharacter.cpp - Hovered Building Component Actor: %s"), *HoveredBuildingComponent->GetName());
			UE_LOG(LogTemp, Warning, TEXT("TimberPlayableCharacter.cpp - Hovered Building Component: %s"), *Hits.GetComponent()->GetName());*/
			FVector2d ScreenLocationOfImpactPoint;

			/*Setting Impact Point at Location to show Delete Widget in Screen Space*/
			GetWorld()->GetFirstPlayerController()->ProjectWorldLocationToScreen(
				Hits.ImpactPoint,
				ScreenLocationOfImpactPoint);

			// Broadcast a Delegate with the Impact Position to the HUD.
			HandleSpawnDeleteIconLocation_DelegateHandle.Broadcast(ScreenLocationOfImpactPoint.X, ScreenLocationOfImpactPoint.Y);
		}
		break;
	}

	//TODO:: Change this to check that if there are no Hits, to Reset the Delete Icon.
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
	if (bIsPlayerDeadNow)
	{
		if (IsValid(PlayerController))
		{
			PlayerController->FlushPressedKeys();		
		}
		//Used for animation bp main state.
		bIsPlayerDead = true;
		
		PlayDeathAnimation();
		//Broadcast to HUD to Update Death UI Reason Text
		//Player Controller && HUD is Subscribed to this Delegate
		HandlePlayerDeath_DelegateHandle.Broadcast(bIsPlayerDeadNow);
	}
}

void ATimberPlayableCharacter::PlayDeathAnimation()
{
	PlayAnimMontage(DeathMontage, 1.f, FName("1"));
}

void ATimberPlayableCharacter::GetPlayerInventoryFromPlayerState()
{
	if (IsValid(PlayerController))
	{
		if (PlayerController->GetPlayerState<APlayerStateBase>()->MainInventory)
		{
			InventoryObject = GetController()->GetPlayerState<APlayerStateBase>()->MainInventory;
		}
	}

	if (!IsValid(InventoryObject))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Character - Inventory Object Not Found."));
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

	if (VignetteComponent)
	{
		VignetteComponent->HandleHealthChange(CurrentHealth/MaxHealth);
	}

	
	if (CurrentHealth <= 0.f)
	{
		bIsPlayerDead = true;
		HandlePlayerDeath(bIsPlayerDead);
	}
	else
	{
		AddOverlayMaterialToCharacter(HitMarkerOverlayMaterial, 0.3f );
	}
}

void ATimberPlayableCharacter::PlayerGainHealth(float HealthAmount)
{
	if (bIsPlayerDead) return;
	if (MaxHealth == CurrentHealth) return;

	if (CurrentHealth + HealthAmount >= MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
	else
	{
		CurrentHealth += HealthAmount;
	}

	if (VignetteComponent)
	{
		VignetteComponent->HandleHealthChange(CurrentHealth/MaxHealth);
		//UE_LOG(LogTemp, Warning, TEXT("Player Gain Health: %f"), CurrentHealth);
	}
	
}

/*Weapon Stuff*/

ETutorialState ATimberPlayableCharacter::GetTutorialState()
{
	ADieRobotGameStateBase* DieRobotGameState = Cast<ADieRobotGameStateBase>(GetWorld()->GetGameState());
	if (DieRobotGameState)
	{
		return DieRobotGameState->TutorialState;
	}

	return ETutorialState::Default;
	
}

void ATimberPlayableCharacter::StopAllAnimMontages()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AnimInstance->StopAllMontages(0.25f);
	}
}

void ATimberPlayableCharacter::PlayWakeAnimationMontage()
{
	if (TutorialWakeMontage)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Player Character - Playing Wake Animation Montage."));
		
		PlayAnimMontage(TutorialWakeMontage, 1.f, FName("WakingUp"));
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Player Character - Wake Animation Montage Not Found."));
	}
}

void ATimberPlayableCharacter::PlayAnimationMontageAtSection(UAnimMontage* MontageToPlay, FName SectionName)
{
	PlayAnimMontage(MontageToPlay, 1.f, SectionName);
}

void ATimberPlayableCharacter::StartLerpRotation(const FRotator& TargetRotation, float DurationOfRotation)
{

	if (DurationOfRotation <= 0)
	{
		//UE_LOG(LogTemp, Error, TEXT("Can Not set a Lerp Rotation Duration of 0."));
		return;
	}

	//Dont want to restart the timer if we are already in the rotation animation.
	if (IsRotating)
	{
		return;
	}
	
	FRotator StartRotation = GetActorRotation().Clamp();
	if (StartRotation == FRotator::ZeroRotator)
	{
		return;
	}

	IsRotating = true;
	
	FTickerDelegate TickDelegate = FTickerDelegate::CreateLambda(
	   [this, StartRotation, TargetRotation, DurationOfRotation](float DeltaTime) -> bool
	   {
		   ElapsedTime += DeltaTime;
		   float Alpha = FMath::Clamp(ElapsedTime / DurationOfRotation, 0.0f, 1.0f);
		   float SmoothedAlpha = FMath::SmoothStep(0.0f, 1.0f, Alpha);

		   FRotator NewRotation = FMath::Lerp(StartRotation, TargetRotation, SmoothedAlpha);
		   SetActorRotation(NewRotation, ETeleportType::TeleportPhysics);

	   		//Early brake if movement started or of animation is finished.
		   if (Alpha >= 1.0f || GetVelocity().Length() > 0.0f)
		   {
			   IsRotating = false;
			   ElapsedTime = 0.0f;
			   return false; // Stop the ticker
		   }
		   return true; // Continue ticking
	   });

	FTSTicker::GetCoreTicker().AddTicker(TickDelegate);

	if (TurnInPlaceMontage)
	{
		StopAnimMontage(TurnInPlaceMontage);
	}
	
}

//CombatComponentAnimUser Interface Override
void ATimberPlayableCharacter::PlayWeaponEquipAnimationMontage(FName SectionName)
{
	//This is this characters-specific implementation of this Override.
	PlayEquipWeaponMontage(SectionName);
}

void ATimberPlayableCharacter::PlayEquipWeaponMontage(FName SectionName)
{
	//Build in function on the ACharacter Class.
	PlayAnimMontage(EquipWeaponMontage, 1.f, SectionName);
}




