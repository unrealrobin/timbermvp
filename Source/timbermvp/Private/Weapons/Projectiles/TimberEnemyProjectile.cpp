// Property of Paracosm Industries. Dont use my shit.


#include "Weapons/Projectiles/TimberEnemyProjectile.h"

#include "Components/CapsuleComponent.h"


// Sets default values
ATimberEnemyProjectile::ATimberEnemyProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if(CapsuleComponent)
	{
		CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ATimberEnemyProjectile::HandleOverlap);
	}
}

// Called when the game starts or when spawned
void ATimberEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATimberEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATimberEnemyProjectile::HandleOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	ATimberPlayableCharacter* PlayerCharacter = Cast<ATimberPlayableCharacter>(OtherActor);

	if(PlayerCharacter)
	{
		PlayerCharacter->PlayerTakeDamage(ProjectileBaseDamage);
		Destroy();
	}
	else
	{
		Destroy();
	}
}

