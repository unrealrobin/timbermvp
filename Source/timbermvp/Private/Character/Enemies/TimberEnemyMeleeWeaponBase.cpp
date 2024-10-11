// Property of Paracosm Industries. Dont use my shit.


#include "Character/Enemies/TimberEnemyMeleeWeaponBase.h"

#include "Weapons/TimberWeaponBase.h"


// Sets default values
ATimberEnemyMeleeWeaponBase::ATimberEnemyMeleeWeaponBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATimberEnemyMeleeWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	EquipMeleeWeapon(MeleeWeaponClassName);
	
}

void ATimberEnemyMeleeWeaponBase::EquipMeleeWeapon(TSubclassOf<ATimberWeaponBase> WeaponClassName)
{
	//TODO:: Spawn Weapon in Hand Slot
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = GetInstigator();

	
	const FTransform RHandSockettTransform = GetMesh()->GetSocketTransform(FName("RHandWeaponSocket"));
	
	AActor* WeaponActor = GetWorld()->SpawnActor<ATimberWeaponBase>(WeaponClassName,
		RHandSockettTransform.GetLocation(), 
		RHandSockettTransform
		.GetRotation().Rotator(),
		SpawnParameters);
	
	EquippedWeapon = Cast<ATimberWeaponBase>(WeaponActor);
	EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RHandWeaponSocket") );
	
}

// Called every frame
void ATimberEnemyMeleeWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

