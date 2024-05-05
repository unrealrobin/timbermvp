// Property of Paracosm Industries. Dont use my shit.


#include "Weapons/Projectiles/TimberProjectileBase.h"

// Sets default values
ATimberProjectileBase::ATimberProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMesh;

	

}

// Called when the game starts or when spawned
void ATimberProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATimberProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

