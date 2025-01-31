// Property of Paracosm Industries. Dont use my shit.


#include "Character/Enemies/TimberEnemyCharacter.h"

#include "AI/TimberAiControllerBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BuildSystem/BuildingComponents/TimberBuildingComponentBase.h"
#include "Character/TimberPlayableCharacter.h"
#include "Character/Enemies/TimberEnemyMeleeWeaponBase.h"
#include "Character/Enemies/TimberEnemyRangedBase.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
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

	GetCharacterMovement()->SetWalkableFloorAngle(70.f);
}

void ATimberEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	/* Listening to Delegate Broadcast from TimberGameMode */
	ATimberGameModeBase* GameMode = Cast<ATimberGameModeBase>(GetWorld()->GetAuthGameMode());
	GameMode->CurrentWaveNumberHandle.AddDynamic(this, &ATimberEnemyCharacter::UpdateCurrentWaveNumber);


}

void ATimberEnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ATimberEnemyCharacter::TakeDamage(float DamageAmount, AActor* DamageInstigator)
{
	//Adding new damage to the accumulated damage during window
	DamageAccumulatedDuringWindow += DamageAmount;
	//Applying damage to Characters Health
	CurrentHealth -= DamageAmount;
	//Starting Damage Accumulation Window
	StartDamageTimerWindow();

	if (CurrentHealth <= 0.f)
	{
		//Checking if the enemy was part of the wave spawn system and thus needs to be tracked.
		GetGameInstance()->GetSubsystem<UWaveGameInstanceSubsystem>()->CheckArrayForEnemy(this);
		ResetDamageWindow();
		//Stops all AI Behavior
		StopAiControllerBehaviorTree();
		OnDeath_HandleCollision();
		PlayMontageAtRandomSection(DeathMontage);
		OnDeath_DropLoot();
	}
	else
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
	GEngine->AddOnScreenDebugMessage(1, 4, FColor::Red, "PlayMeleeHitSoundCalled.");

	FVector HitLocation = HitResult.ImpactPoint;
	if (MeleeHitSound)
	{
		GEngine->AddOnScreenDebugMessage(1, 4, FColor::Red, "Playing Hit Sound.");
		UGameplayStatics::PlaySoundAtLocation(this, MeleeHitSound, HitLocation);
	}
}

void ATimberEnemyCharacter::OnDeath_HandleCollision()
{
	//Disable Collision
	TArray<USceneComponent*> CollisionComponents;
	GetRootComponent()->GetChildrenComponents(true, CollisionComponents);
	CollisionComponents.Add(GetRootComponent());
	for (USceneComponent* Component : CollisionComponents)
	{
		if (UShapeComponent* ShapeComponent = Cast<UShapeComponent>(Component))
		{
			/*Remove All Collisions but don't fall through the map.*/
			ShapeComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
			ShapeComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
			ShapeComponent->SetCanEverAffectNavigation(false);
			ShapeComponent->bFillCollisionUnderneathForNavmesh = true;
		}
		else if (UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(Component))
		{
			StaticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
			StaticMesh->SetCanEverAffectNavigation(false);
			StaticMesh->bFillCollisionUnderneathForNavmesh = true;
		}
		//UE_LOG(LogTemp, Warning, TEXT("Collision Disabled for a Single Component"));
	}
}

void ATimberEnemyCharacter::OnDeath_DropLoot()
{
	//When the enemy dies, it drops loot that the player can pick up.
	//Can be overwritten by child Classes.

	//Goes through each item in the array and spawns the loot.
	//100% Chance now
	//TODO:: Add a random chance to spawn loot for each Loot Item.
	for(TSubclassOf<AEnemyLootDropBase> LootDrop : StandardLootArray)
	{
		if(LootDrop)
		{
			SpawnLoot(LootDrop);
		}
	}
}

void ATimberEnemyCharacter::SpawnLoot(TSubclassOf<AEnemyLootDropBase> LootDropClass)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* LootItem = GetWorld()->SpawnActor<AEnemyLootDropBase>(LootDropClass,
		GetActorLocation(),
		GetActorRotation(), 
		SpawnParams);
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

//TODO::Why is the Current Wave Number important on the BaseEnemyClass?
void ATimberEnemyCharacter::UpdateCurrentWaveNumber(float CurrentWaveNumber)
{
	CurrentWave = CurrentWaveNumber;
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
