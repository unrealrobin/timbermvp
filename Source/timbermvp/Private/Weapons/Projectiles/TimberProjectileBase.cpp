// Property of Paracosm Industries. Dont use my shit.


#include "Weapons/Projectiles/TimberProjectileBase.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ATimberProjectileBase::ATimberProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	RootComponent = CapsuleComponent;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement Component");
	

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

