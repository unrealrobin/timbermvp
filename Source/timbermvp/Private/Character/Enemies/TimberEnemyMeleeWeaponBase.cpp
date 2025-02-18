// Property of Paracosm Industries. Dont use my shit.


#include "Character/Enemies/TimberEnemyMeleeWeaponBase.h"

#include "Weapons/TimberWeaponBase.h"


// Sets default values
ATimberEnemyMeleeWeaponBase::ATimberEnemyMeleeWeaponBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyType = EEnemyType::MeleeWeaponRobot;
}

// Called when the game starts or when spawned
void ATimberEnemyMeleeWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	EquipMeleeWeapon(MeleeWeaponClassName);
}

void ATimberEnemyMeleeWeaponBase::EquipMeleeWeapon(TSubclassOf<ATimberWeaponBase> WeaponClassName)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = Cast<APawn>(this);

	if (GetMesh())
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::Black, "Mesh Found for Melee Weapon Spawn.");

		const FTransform RHandSocketTransform = GetMesh()->GetSocketTransform("RHandWeaponSocket");
		if (RHandSocketTransform.IsValid())
		{
			GEngine->AddOnScreenDebugMessage(2, 5.0, FColor::Black, "Right Hand Socket Transform Found");

			AActor* WeaponActor = GetWorld()->SpawnActor<ATimberWeaponBase>(
				WeaponClassName,
				RHandSocketTransform.GetLocation(),
				RHandSocketTransform
				.GetRotation().Rotator(),
				SpawnParameters);

			EquippedWeapon = Cast<ATimberWeaponBase>(WeaponActor);
			if (EquippedWeapon)
			{
				EquippedWeapon->AttachToComponent(
					GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "RHandWeaponSocket");
				EnemyWeaponType = EEnemyWeaponState::MeleeWeaponEquipped;
			}
		}
	}
}

// Called every frame
void ATimberEnemyMeleeWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
