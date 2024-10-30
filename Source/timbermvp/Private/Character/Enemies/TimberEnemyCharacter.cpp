// Property of Paracosm Industries. Dont use my shit.


#include "Character/Enemies/TimberEnemyCharacter.h"
#include "BuildSystem/TimberBuildingComponentBase.h"
#include "Character/TimberPlayableCharacter.h"
#include "Character/Enemies/TimberEnemyMeleeWeaponBase.h"
#include "Character/Enemies/TimberEnemyRangedBase.h"
#include "GameModes/TimberGameModeBase.h"
#include "Weapons/TimberWeaponRangedBase.h"

ATimberEnemyCharacter::ATimberEnemyCharacter()
{
	RaycastStartPoint = CreateDefaultSubobject<USceneComponent>("RaycastStartPoint");
	RaycastStartPoint->SetupAttachment(RootComponent);
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

void ATimberEnemyCharacter::TakeDamage(float DamageAmount)
{
	CurrentHealth -= DamageAmount;

	//Used for AI Damage/Aggro System
	//If the player has dealt more than 20 damage to the enemy, the enemy will aggro the player. Causing the BB Value to Change
	if(MaxHealth - CurrentHealth > 20.f)
	{
		bHasBeenAggroByPlayer = true;
	}

	if(CurrentHealth <= 0.f)
	{
		//Checking if the enemy was part of the wave spawn system and thus needs to be tracked.
		ATimberGameModeBase* GameMode = Cast<ATimberGameModeBase>(GetWorld()->GetAuthGameMode());
		GameMode->CheckArrayForEnemy(this);

		//Plays the Death Animation that Calls the Destroy Function From an Event Notify
		PlayMontageAtRandomSection(DeathMontage);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Target hit for: %f. CurrentHealth: %f."), DamageAmount, CurrentHealth);
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

	if(MeleeWeaponEnemy)
	{
		MeleeWeaponEnemy->EquippedWeapon->Destroy();
	}

	if(RangedWeaponEnemy)
	{
		RangedWeaponEnemy->EquippedWeapon->Destroy();
	}
}

float ATimberEnemyCharacter::CalculateOutputDamage(float Damage)
{
	return Damage;
}


void ATimberEnemyCharacter::PlayMontageAtRandomSection(UAnimMontage* Montage)
{
	int NumberOfMontageSections = Montage->GetNumSections();
	int RandomSection = FMath::RandRange(0, NumberOfMontageSections - 1);
	PlayAnimMontage(Montage, 1, Montage->GetSectionName(RandomSection));
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

	for(FHitResult HitActors:  HitResults)
	{
		if(HitActors.GetActor()->IsA(ATimberBuildingComponentBase::StaticClass()))
		{
			return Cast<ATimberBuildingComponentBase>(HitActors.GetActor());
		}
	}

	return nullptr;
	
}