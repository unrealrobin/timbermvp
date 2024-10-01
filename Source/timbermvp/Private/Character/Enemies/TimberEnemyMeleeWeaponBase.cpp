// Property of Paracosm Industries. Dont use my shit.


#include "Character/Enemies/TimberEnemyMeleeWeaponBase.h"


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

	SpawnMeleeWeapon(MeleeWeaponClassName);
	
}

void ATimberEnemyMeleeWeaponBase::SpawnMeleeWeapon(TSubclassOf<ATimberWeaponBase> WeaponClassName)
{
	//TODO:: Spawn Weapon in Hand Slot
	
}

// Called every frame
void ATimberEnemyMeleeWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

