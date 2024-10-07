// Property of Paracosm Industries. Dont use my shit.


#include "Weapons/TimberWeaponRangedBase.h"


// Sets default values
ATimberWeaponRangedBase::ATimberWeaponRangedBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATimberWeaponRangedBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATimberWeaponRangedBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

