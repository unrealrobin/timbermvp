// Property of Paracosm Industries. Dont use my shit.


#include "Weapons/Projectiles/TimberPlayerProjectile.h"

#include "Components/CapsuleComponent.h"
#include "Interfaces/DamageableEnemy.h"


class IDamageableEnemy;
// Sets default values
ATimberPlayerProjectile::ATimberPlayerProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Overlap Delegate
	if (CapsuleComponent)
	{
		CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ATimberPlayerProjectile::HandleOverlap);
	}
}

// Called when the game starts or when spawned
void ATimberPlayerProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATimberPlayerProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATimberPlayerProjectile::HandleOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	IDamageableEnemy* HitEnemy = Cast<IDamageableEnemy>(OtherActor);

	if (HitEnemy)
	{
		//Play the IDamageableEnemy's TakeDamage function. Interface.
		HitEnemy->PlayProjectileHitSound(SweepResult);
		HitEnemy->TakeDamage(25);

		//Destroys the projectile on hitting an enemy that may take damage from this projectile.
		Destroy();
	}
}
