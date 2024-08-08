// Property of Paracosm Industries. Dont use my shit.


#include "Weapons/Projectiles/TimberProjectileBase.h"

#include "Character/TimberPlayableCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interfaces/DamageableEnemy.h"

ATimberProjectileBase::ATimberProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	RootComponent = CapsuleComponent;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(RootComponent);
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement Component");

	
}

void ATimberProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	ProjectileOwner = Cast<ATimberPlayableCharacter>(GetOwner());
	
	//Overlap Delegate
	if(CapsuleComponent)
	{
		CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ATimberProjectileBase::HandleOverlap);
	}
}

void ATimberProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATimberProjectileBase::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	
	IDamageableEnemy* HitEnemy = Cast<IDamageableEnemy>(OtherActor);
	ATimberPlayableCharacter* TimberCharacter = Cast<ATimberPlayableCharacter>(GetOwner());
	
	if(HitEnemy)
	{
		HitEnemy->TakeDamage(20);
		Destroy();
	}
	else // If overlapped with something else, destroy the projectile
	{
		Destroy();
	}
}

