// Property of Paracosm Industries.


#include "Character/Enemies/TimberEnemyCharacter.h"
#include "AI/TimberAiControllerBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BuildSystem/BuildingComponents/TimberBuildingComponentBase.h"
#include "Character/TimberPlayableCharacter.h"
#include "Character/Enemies/TimberEnemyMeleeWeaponBase.h"
#include "Character/Enemies/TimberEnemyRangedBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/StatusEffect/StatusEffectHandlerComponent.h"
#include "Data/DataAssets/LootTable.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameModes/TimberGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Loot/EnemyLootDropBase.h"
#include "Sound/SoundCue.h"
#include "Subsystems/Wave/WaveGameInstanceSubsystem.h"
#include "Weapons/TimberWeaponRangedBase.h"

ATimberEnemyCharacter::ATimberEnemyCharacter()
{
	RaycastStartPoint = CreateDefaultSubobject<USceneComponent>("RaycastStartPoint");
	RaycastStartPoint->SetupAttachment(RootComponent);
	
	StatusEffectHandler = CreateDefaultSubobject<UStatusEffectHandlerComponent>("StatusEffectHandler");

	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		CharacterMovementComponent->SetWalkableFloorAngle(56.0f);
		CharacterMovementComponent->MaxWalkSpeed = MaxWalkSpeedBase;
		CharacterMovementComponent->NavAgentProps.AgentRadius = 100.0f;
		CharacterMovementComponent->NavAgentProps.AgentHeight = 180.0f;
		CharacterMovementComponent->NavAgentProps.AgentStepHeight = 65.0f;
		CharacterMovementComponent->bUseRVOAvoidance = true;
		CharacterMovementComponent->AvoidanceConsiderationRadius = 600.0f;
		CharacterMovementComponent->AvoidanceWeight = 0.8f;
	}
}

void ATimberEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATimberEnemyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ATimberEnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ATimberEnemyCharacter::TakeDamage(float DamageAmount, AActor* DamageInstigator)
{
	//TODO::PlayHitReact Montage
	PlayAnimMontage(HitReactMontage, 1, FName("Back"));
	
	//Glows the Enemy Character briefly when hit.
	AddOverlayMaterialToCharacter(HitMarkerOverlayMaterial, 0.3f);
	
	//Adding new damage to the accumulated damage during window
	DamageAccumulatedDuringWindow += DamageAmount;
	//Applying damage to Characters Health
	CurrentHealth -= DamageAmount;
	//Starting Damage Accumulation Window / Used for Aggro Conditions
	StartDamageTimerWindow();

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
		bHasBeenAggroByPlayer = HandleAggroCheck(DamageInstigator, DamageAmount, DamageAccumulatedDuringWindow);
		//UE_LOG(LogTemp, Warning, TEXT("Target hit for: %f. CurrentHealth: %f."), DamageAmount, CurrentHealth);
	}
}

bool ATimberEnemyCharacter::HandleAggroCheck(AActor* DamageInstigator, float DamageReceived, float fDamageAccumulatedDuringWindow)
{
	//If DamageInstigator is a player character, check if the enemy should aggro the player.
	if (ATimberPlayableCharacter* PlayerCharacter = Cast<ATimberPlayableCharacter>(DamageInstigator))
	{
		// If damage accumulated during window is greater than 20 or the enemy has lost more than 40% of its health, it will aggro the player.
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
		// Starts the Damage Window Timer that starts at first hit for (X: DamageWindowTime ) and automatically resets itself at the end of the window.
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
	if (ProjectileHitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ProjectileHitSound, HitLocation);
	}
}

void ATimberEnemyCharacter::PlayMeleeWeaponHitSound(FHitResult HitResult)
{
	//GEngine->AddOnScreenDebugMessage(1, 4, FColor::Red, "PlayMeleeHitSoundCalled.");

	FVector HitLocation = HitResult.ImpactPoint;
	if (MeleeHitSound)
	{
		//GEngine->AddOnScreenDebugMessage(1, 4, FColor::Red, "Playing Hit Sound.");
		UGameplayStatics::PlaySoundAtLocation(this, MeleeHitSound, HitLocation);
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

void ATimberEnemyCharacter::HandleRemoveStatusEffectComponent()
{
	if (StatusEffectHandler)
	{
		StatusEffectHandler->DestroyComponent();
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

	for (FHitResult HitActors : HitResults)
	{
		if (HitActors.GetActor()->IsA(ATimberBuildingComponentBase::StaticClass()))
		{
			return Cast<ATimberBuildingComponentBase>(HitActors.GetActor());
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
