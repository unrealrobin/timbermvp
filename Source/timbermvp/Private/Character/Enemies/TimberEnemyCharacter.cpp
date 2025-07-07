// Property of Paracosm Industries.


#include "Character/Enemies/TimberEnemyCharacter.h"

#include "MetasoundSource.h"
#include "AI/TimberAiControllerBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BuildSystem/BuildingComponents/TimberBuildingComponentBase.h"
#include "Character/TimberPlayableCharacter.h"
#include "Character/Enemies/TimberEnemyMeleeWeaponBase.h"
#include "Character/Enemies/TimberEnemyRangedBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/Navigation/NavigationHelperComponent.h"
#include "Components/StatusEffect/StatusEffectHandlerComponent.h"
#include "Controller/TimberPlayerController.h"
#include "Data/DataAssets/LootTable.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameModes/TimberGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Loot/EnemyLootDropBase.h"
#include "Sound/SoundCue.h"
#include "Subsystems/Wave/WaveGameInstanceSubsystem.h"
#include "UI/FloatingDamageNumbers/FloatingDamageContainer.h"
#include "UI/StatusEffects/StatusEffectBar.h"
#include "Weapons/TimberWeaponRangedBase.h"


ATimberEnemyCharacter::ATimberEnemyCharacter()
{
	RaycastStartPoint = CreateDefaultSubobject<USceneComponent>("RaycastStartPoint");
	RaycastStartPoint->SetupAttachment(RootComponent);

	StatusEffectBarComponent = CreateDefaultSubobject<UWidgetComponent>("StatusEffectBar");
	StatusEffectBarComponent->SetupAttachment(RootComponent);
	
	DamageEffectUISpawnPoint = CreateDefaultSubobject<USceneComponent>("DamageEffectUISpawnPoint");
	DamageEffectUISpawnPoint->SetupAttachment(RootComponent);

	StatusEffectHandler = CreateDefaultSubobject<UStatusEffectHandlerComponent>("StatusEffectHandler");
	NavHelperComponent = CreateDefaultSubobject<UNavigationHelperComponent>("NavHelperComponent");

	
}

void ATimberEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetupStatusEffectBar();
	
	SetupCharacterMovementDelegates();
	
	SetupCharacterMovementData();

	ScaleHealth();
}

void ATimberEnemyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ATimberEnemyCharacter::SelfDestruct()
{
	//TODO:: Add VFX for Destruction of Enemy.
	//Potentially need to delay the rest of this logic for the end of the Destruction VFX.

	
	StopAiControllerBehaviorTree();

	//We want this actor not collidable so nothing interrupts the Montage about to play.
	OnDeath_HandleCollision();
	
	//Potentially not necessary as destruction completely stops the AI Controller.
	HandleRemoveStatusEffectComponent();

	//Adjusts the Wave Enemy Count Number
	GetGameInstance()->GetSubsystem<UWaveGameInstanceSubsystem>()->CheckArrayForEnemy(this);


	//Calling Destroy in Animation Notify on this Montage.
	if (SelfDestructMontage)
	{
		PlayAnimMontage(SelfDestructMontage, 1);
	}
	

	UE_LOG(LogTemp, Warning, TEXT("Actor Self Destructed."));
}

void ATimberEnemyCharacter::SpawnDamageUI(float DamageAmount)
{
	if (FloatingDamageContainerClass)
	{
		FActorSpawnParameters SpawnParams;
		AActor* FloatingDamageActor = GetWorld()->SpawnActor<AActor>(FloatingDamageContainerClass,
			DamageEffectUISpawnPoint->GetComponentLocation(),
			FRotator::ZeroRotator, 
			SpawnParams);

		if (AFloatingDamageContainer* FloatingDamage = Cast<AFloatingDamageContainer>(FloatingDamageActor))
		{
			FloatingDamage->SetDamageAmount(DamageAmount);
		}
		
	}
}

void ATimberEnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ATimberEnemyCharacter::SetupCharacterMovementData()
{
	if (CharacterMovementComponent)
	{
		CharacterMovementComponent->SetWalkableFloorAngle(65.0f);
		CharacterMovementComponent->MaxWalkSpeed = MaxWalkSpeedBase;
		CharacterMovementComponent->MaxStepHeight = 60.0f;

		//Used to not slow down inbetween Nav Points.
		CharacterMovementComponent->BrakingDecelerationWalking = 0.0f;
		
		CharacterMovementComponent->NavAgentProps.AgentRadius = 34.0f;
		CharacterMovementComponent->NavAgentProps.AgentHeight = 180.0f;
		CharacterMovementComponent->NavAgentProps.AgentStepHeight = 60.0f;

		//TODO:: Check a way of Reimplementing this in the future. RVO may be beneficial for this. 
		CharacterMovementComponent->bUseRVOAvoidance = false;
		CharacterMovementComponent->AvoidanceConsiderationRadius = 2000.0f;
		CharacterMovementComponent->AvoidanceWeight = 0.5f;
	}
}

void ATimberEnemyCharacter::SetupCharacterMovementDelegates()
{
	CharacterMovementComponent = GetCharacterMovement();
	if (CharacterMovementComponent)
	{
		MovementModeChangedDelegate.AddDynamic(this, &ATimberEnemyCharacter::HandleOnMovementModeChanged);
		LandedDelegate.AddDynamic(this, &ATimberEnemyCharacter::HandleOnLanded);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Character Movement Component is NULL."));
	}
}

void ATimberEnemyCharacter::HandleOnMovementModeChanged(class ACharacter* Character, EMovementMode PrevMovementMode,
	uint8 PreviousCustomMode)
{
	if (Character)
	{
		ATimberAiControllerBase* AiController = Cast<ATimberAiControllerBase>(GetController());
		if (AiController)
		{
			switch (Character->GetCharacterMovement()->MovementMode.GetValue())
			{
			case MOVE_Falling:
				AiController->BehaviorTreeComponent->StopLogic("Enemy Character is Falling.");
				break;
			default:
				// Handle other movement modes if necessary
				break;
			}
		}
	}
}

void ATimberEnemyCharacter::HandleOnLanded(const FHitResult& Hit)
{
	//Ai Controller to Restart Ai Controller Brain - this should restart the pathing solution.
	ATimberAiControllerBase* AiController = Cast<ATimberAiControllerBase>(GetController());
	if (AiController)
	{
		//Restarting the Behavior tree Logic if the AI Controller is not already running.
		if (AiController->BehaviorTreeComponent && !AiController->BehaviorTreeComponent->IsRunning())
		{
			AiController->BehaviorTreeComponent->StartLogic();
		}
	}
}

void ATimberEnemyCharacter::TakeDamage(float DamageAmount, AActor* DamageInstigator)
{
	if (DamageInstigator)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyCharacter - TakeDamage() - %s took %f damage from %s."), *GetName(), DamageAmount, *DamageInstigator->GetName());
	}
	
	//Applying damage to Character Health
	CurrentHealth -= DamageAmount;

	SpawnDamageUI(DamageAmount);
	
	//Chance for Hit React to Play - Hit Reacts Interrupt other Montages like Attack Montage so we want to limit it.
	float RandFloat = FMath::FRandRange(0.0f, 10.0f);
	if (RandFloat > 9.0f)
	{
		PlayAnimMontage(HitReactMontage, 1, FName("Back"));
	}
	
	//Glows the Enemy Character briefly when hit.
	AddOverlayMaterialToCharacter(HitMarkerOverlayMaterial, 0.3f);
	
	//Adding new damage to the accumulated damage window
	DamageAccumulatedDuringWindow += DamageAmount;
	
	//Starting Damage Accumulation Window / Used for Aggro Conditions
	StartDamageTimerWindow();

	//Things to do after Death - 0 Life.
	if (CurrentHealth <= 0.f)
	{
		HandleRemoveStatusEffectComponent();
		
		//Checking if the enemy was part of the wave spawn system and thus needs to be tracked.
		GetGameInstance()->GetSubsystem<UWaveGameInstanceSubsystem>()->CheckArrayForEnemy(this);

		//Clears Aggro and Damage Set on the Damage Accumulation Window
		ResetDamageWindow();
		
		//Stops all AI Behavior
		StopAiControllerBehaviorTree();

		//Handles Collisions so the enemy cant be attacked more or block navigation
		OnDeath_HandleCollision();

		//Plays Death Animation
		if (DeathMontage)
		{
			PlayMontageAtRandomSection(DeathMontage);
		}

		//Drops Any Loot set on the Characters Loot Drop
		OnDeath_DropLoot();
	}
	else if (DamageInstigator)
	{
		//TODO:: Potential Removed from Game. - Delete Later is Unused still.
		bHasBeenAggroByPlayer = HandleAggroCheck(DamageInstigator, DamageAmount, DamageAccumulatedDuringWindow);
		//UE_LOG(LogTemp, Warning, TEXT("Target hit for: %f. CurrentHealth: %f."), DamageAmount, CurrentHealth);
	}
}

bool ATimberEnemyCharacter::HandleAggroCheck(AActor* DamageInstigator, float DamageReceived, float fDamageAccumulatedDuringWindow)
{
	//If DamageInstigator is a player character, check if the enemy should aggro the player.
	if (Cast<ATimberPlayableCharacter>(DamageInstigator))
	{
		// If damage accumulated during a damage window is greater than 20 or the enemy has lost more than 40% of its health, it will then aggro the player.
		if(DamageReceived > (MaxHealth * .40) || fDamageAccumulatedDuringWindow > 20 )
		{
			return true;
		}
	}
	
	return false;
}

void ATimberEnemyCharacter::ResetDamageWindow()
{
	DamageAccumulatedDuringWindow = 0.f;
}

void ATimberEnemyCharacter::StartDamageTimerWindow()
{
	bool bIsTimerActive = GetWorld()->GetTimerManager().IsTimerActive(DamageWindowTimerHandle);
	if(!bIsTimerActive)
	{
		// Starts the Damage Window Timer that starts at first hit for (X: DamageWindowTime) and automatically resets itself at the end of the window.
		GetWorld()->GetTimerManager().SetTimer(DamageWindowTimerHandle, this, &ATimberEnemyCharacter::ResetDamageWindow, DamageWindowTime, false);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Damage Window timer is already active."))
	}
}

void ATimberEnemyCharacter::PlayProjectileHitSound(FHitResult HitResult)
{
	FVector HitLocation = HitResult.ImpactPoint;
	if (ProjectileHitSound_MetaSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ProjectileHitSound_MetaSound, HitLocation);
	}
}

void ATimberEnemyCharacter::PlayMeleeWeaponHitSound(FHitResult HitResult)
{
	//GEngine->AddOnScreenDebugMessage(1, 4, FColor::Red, "PlayMeleeHitSoundCalled.");

	FVector HitLocation = HitResult.ImpactPoint;
	if (MeleeHitSound_MetaSound)
	{
		//GEngine->AddOnScreenDebugMessage(1, 4, FColor::Red, "Playing Hit Sound.");
		UGameplayStatics::PlaySoundAtLocation(this, MeleeHitSound_MetaSound, HitLocation);
	}
}

void ATimberEnemyCharacter::OnDeath_HandleCollision()
{
	GetCapsuleComponent()->SetCollisionProfileName("DR_DeadCharacter");
}

void ATimberEnemyCharacter::OnDeath_DropLoot()
{
	//UE_LOG(LogTemp, Warning, TEXT("Enemy Destroyed. Dropping Loot."))
	//Get the Loot Table
	if (LootTable)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Loot Table is Valid."))
		//Loot Table has an array that will be looped over.
		TArray<FLootEntry> LootEntries = LootTable->LootEntries;
		//Each item in the Array is a FLootEntry Struct that has a TSubclassOf<AEnemyLootDropBase> and a TMap<int, float> AmountChance.
		if (LootEntries.Num() > 0)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Found Loot Entries."))
			//We loop over the array, and use the Structs PickAmount() function to get a random amount of loot to drop.
			for (FLootEntry Items : LootEntries)
			{

				//This breaks this loop iteration as the chance was not met.
				//Checking if any loot should drop at all.Drop Chance set in the Loot Table.
				float AnyLootDropChance = FMath::FRand();
				//UE_LOG(LogTemp, Warning, TEXT("AnyLootDropChance: %f. Item Drop Chance: %f."), AnyLootDropChance, Items.DropChance);
				if (AnyLootDropChance > Items.DropChance)
				{
					continue;
				}

				FActorSpawnParameters SpawnParams;
				int32 AmountToSpawn = Items.PickAmount();
				//UE_LOG(LogTemp, Warning, TEXT("Amount to Spawn: %d"), AmountToSpawn);
				//We then spawn the loot using the SpawnLoot() function.
				for (int i = 0; i < AmountToSpawn; i++)
				{
					AActor* SpawnedLootActor = GetWorld()->SpawnActor<AActor>(Items.LootItemClass,
						GetActorLocation(),
						FRotator::ZeroRotator, 
						SpawnParams);

					if (SpawnedLootActor)
					{
						// Try to apply an upward impulse
						UPrimitiveComponent* PrimitiveComp = Cast<UPrimitiveComponent>(SpawnedLootActor->GetRootComponent());
						if (PrimitiveComp && PrimitiveComp->IsSimulatingPhysics())
						{
							FVector Impulse = FVector(
								FMath::FRandRange(-100.0f, 100.0f), // small random left/right push
								FMath::FRandRange(-100.0f, 100.0f), // small random forward/backward push
								FMath::FRandRange(400.0f, 600.0f)   // strong upward push
							);

							PrimitiveComp->AddImpulse(Impulse, NAME_None, false);
						}//UE_LOG(LogTemp, Warning, TEXT("Spawned Loot Actor: %s"), *SpawnedLootActor->GetName());
					}
				}
			}
		}
	}
}

void ATimberEnemyCharacter::ScaleHealth()
{
	UWaveGameInstanceSubsystem* WaveGameInstance = GetGameInstance()->GetSubsystem<UWaveGameInstanceSubsystem>();
	if (WaveGameInstance)
	{
		float WaveNum = WaveGameInstance->CurrentWaveNumber;

		//Using a Temp Var here because it can be changed per BP in Editor.
		float BaseMaxHealth = MaxHealth;

		/*
		 * Wave 1 = 0
		 * Wave 2 = 3.5
		 * Wave 3 = 7
		 */

		float AdjustmentValue;

		if (WaveNum < 7)
		{
			AdjustmentValue = 3.5f;
		}
		else if (WaveNum < 12)
		{
			AdjustmentValue = 7.5f;
		}
		else if (WaveNum < 12)
		{
			AdjustmentValue = 9.5f;
		}
		else if (WaveNum < 18)
		{
			AdjustmentValue = 11.5f;
		}
		else if (WaveNum < 23)
		{
			AdjustmentValue = 13.5f;
		}
		else
		{
			AdjustmentValue = 15.5f;
		}
		
		
		float ScaleValue = (WaveNum - 1) * AdjustmentValue / 100;
		float ScaledHealth = BaseMaxHealth + (BaseMaxHealth * ScaleValue);

		MaxHealth = ScaledHealth;
		CurrentHealth = ScaledHealth;

		UE_LOG(LogTemp, Warning, TEXT("Base Health: %f. Scaled Health: %f, Scale Value: %f."), BaseMaxHealth, ScaledHealth, ScaleValue);
	}
	
}

void ATimberEnemyCharacter::HandleRemoveStatusEffectComponent()
{
	if (StatusEffectHandler)
	{
		StatusEffectHandler->DestroyComponent();
	}
}

void ATimberEnemyCharacter::HandleToggleDataView(FInputActionValue Input)
{
	/*When holding down Tab
	 * Should show All Data - Status Effect Bar
	 * If not holding, hide the Status Effect Bar
	 */
	if (Input.Get<bool>()) //true
	{
		if (StatusEffectBarComponent && StatusEffectBarComponent->GetUserWidgetObject() && StatusEffectBarComponent->GetUserWidgetObject()->GetVisibility() == ESlateVisibility::Hidden)
		{
			//Toggles the visibility of the Status Effect Bar
			StatusEffectBarComponent->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Visible);
			
		}
	}
	else //false
	{
		if (StatusEffectBarComponent && StatusEffectBarComponent->GetUserWidgetObject() && StatusEffectBarComponent->GetUserWidgetObject()->GetVisibility() == ESlateVisibility::Visible)
		{
			StatusEffectBarComponent->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ATimberEnemyCharacter::HandleEnemyDeath()
{
	HandleWeaponDestruction();
	Destroy();
}

void ATimberEnemyCharacter::HandleWeaponDestruction()
{
	ATimberEnemyMeleeWeaponBase* MeleeWeaponEnemy = Cast<ATimberEnemyMeleeWeaponBase>(this);
	ATimberEnemyRangedBase* RangedWeaponEnemy = Cast<ATimberEnemyRangedBase>(this);

	if (MeleeWeaponEnemy)
	{
		if (MeleeWeaponEnemy->EquippedWeapon)
		{
			MeleeWeaponEnemy->EquippedWeapon->Destroy();
		}
	}

	if (RangedWeaponEnemy)
	{
		if (RangedWeaponEnemy->EquippedWeapon)
		{
			RangedWeaponEnemy->EquippedWeapon->Destroy();
		}
	}
}

float ATimberEnemyCharacter::CalculateOutputDamage(float Damage)
{
	return Damage;
}

void ATimberEnemyCharacter::StopAiControllerBehaviorTree()
{
	ATimberAiControllerBase* AiController = Cast<ATimberAiControllerBase>(GetController());
	if (AiController && AiController->BehaviorTreeComponent)
	{
		//Does not abort the current task, but does FinishtheTask
		AiController->BehaviorTreeComponent->StopLogic("Enemy has been killed");
	}
}

void ATimberEnemyCharacter::PlayMontageAtRandomSection(UAnimMontage* Montage)
{
	if(Montage)
	{
		int NumberOfMontageSections = Montage->GetNumSections();
		int RandomSection = FMath::RandRange(0, NumberOfMontageSections - 1);
		PlayAnimMontage(Montage, 1, Montage->GetSectionName(RandomSection));
	}
	
}

ATimberBuildingComponentBase* ATimberEnemyCharacter::LineTraceToSeeda()
{
	FVector RaycastStart = RaycastStartPoint->GetComponentLocation();

	//Get Seeda Location
	FVector RaycastEnd = Cast<ATimberGameModeBase>(GetWorld()->GetAuthGameMode())->SeedaLocation;

	TArray<FHitResult> HitResults;

	bool bHit = GetWorld()->LineTraceMultiByChannel(
		HitResults,
		RaycastStart,
		RaycastEnd,
		ECC_Visibility);

	if (bHit)
	{
		for (FHitResult HitActors : HitResults)
		{
			if (HitActors.GetActor()->IsA(ATimberBuildingComponentBase::StaticClass()))
			{
				return Cast<ATimberBuildingComponentBase>(HitActors.GetActor());
			}
		}
	}

	return nullptr;
}

void ATimberEnemyCharacter::SweepForActor(TSubclassOf<AActor> ActorToSweepFor, float SphereRadius, float DistanceToCheck)
{
	FCollisionShape Sphere = FCollisionShape::MakeSphere(SphereRadius);
	FVector SweepStart = RaycastStartPoint->GetComponentLocation();
	FVector SweepEnd = SweepStart + RaycastStartPoint->GetForwardVector() * DistanceToCheck;
	
	TArray<FHitResult> HitResults;
	bool bHit = GetWorld()->SweepMultiByChannel(HitResults,
		SweepStart,
		SweepEnd,
		FQuat::Identity,
		ECC_Visibility,
		Sphere );

	//UE_LOG(LogTemp, Warning, TEXT("Swept for Actor."))
	
	if (bHit)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Actor Type Found In Array"));
		for (FHitResult Hit : HitResults)
		{
			if (Hit.GetActor()->IsA(ActorToSweepFor))
			{
				UE_LOG(LogTemp, Warning, TEXT("Actor Found: %p"), Hit.GetActor()->GetClass());
				SweepHitActor = Hit.GetActor();
				//DrawDebugSphere(GetWorld(), SweepHitActor->GetActorLocation(), 100.f, 12, FColor::Red, false, 3.f);
				break;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Actor Found"));
	}
}

void ATimberEnemyCharacter::SetupStatusEffectBar()
{
	if (StatusEffectBarComponent && StatusEffectBarComponent->GetWidget())
	{
		StatusEffectBarComponent->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
	}
	
	UStatusEffectBar* StatusEffectBarWidget = Cast<UStatusEffectBar>(StatusEffectBarComponent->GetWidget());
	if (StatusEffectBarWidget)
	{
		ATimberPlayableCharacter* PlayerCharacter = Cast<ATimberPlayableCharacter>(UGameplayStatics::GetActorOfClass(this, ATimberPlayableCharacter::StaticClass()));
		if (PlayerCharacter)
		{
			ATimberPlayerController* PC = Cast<ATimberPlayerController>(PlayerCharacter->GetController());
			if (PC)		
			{
				PC->ToggleDataView_DelegateHandle.AddDynamic(this, &ATimberEnemyCharacter::HandleToggleDataView);
			}
		}
	}
}


