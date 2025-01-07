// Property of Paracosm Industries. Dont use my shit.


#include "Weapons/TimberWeaponBase.h"

#include "ActorReferencesUtils.h"
#include "Character/TimberPlayableCharacter.h"
#include "Character/Enemies/TimberEnemyCharacter.h"
#include"Weapons/Projectiles/TimberProjectileBase.h"
#include "Components/BoxComponent.h"


// Sets default values
ATimberWeaponBase::ATimberWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void ATimberWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}
