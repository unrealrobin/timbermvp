// Property of Paracosm Industries. Dont use my shit.


#include "Weapons/Projectiles/TimberEnemyProjectile.h"

#include "BuildSystem/BuildableBase.h"
#include "BuildSystem/BuildingComponents/TimberBuildingComponentBase.h"
#include "Character/TimberSeeda.h"
#include "Components/CapsuleComponent.h"


// Sets default values
ATimberEnemyProjectile::ATimberEnemyProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (CapsuleComponent)
	{
		CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ATimberEnemyProjectile::HandleOverlap);
		CapsuleComponent->OnComponentHit.AddDynamic(this, &ATimberEnemyProjectile::HandleBlocked);
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

void ATimberEnemyProjectile::HandleBlocked(
	UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (Cast<ATimberBuildingComponentBase>(OtherActor))
	{
		ATimberBuildingComponentBase* BuildingComponent = Cast<ATimberBuildingComponentBase>(OtherActor);
		//If the projectile is blocked by a BuildingComponentBase (Wall, Floor, Ramp), damage the BuildingComponent.
		//Traps and constructs don't take damage from enemy projectiles yet.
		BuildingComponent->BuildingComponentTakeDamage(ProjectileBaseDamage, this);
		Destroy();
	}
	else
	{
		//Blocked by a non-damageable object. Destroy the projectile.
		Destroy();
		UE_LOG(LogTemp, Warning, TEXT("EnemyProjectile - Enemy Projectile has been Blocked and Destroyed - No Damage."))
	}
}

void ATimberEnemyProjectile::HandleOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	ATimberPlayableCharacter* PlayerCharacter = Cast<ATimberPlayableCharacter>(OtherActor);
	ATimberSeeda* Seeda = Cast<ATimberSeeda>(OtherActor);

	//Enemy Projectile Damage is Output to the Player Character or Seeda's Take damage functions -
	//Enemy Damage Reduction Principles are applied in their own functions.
	
	if (PlayerCharacter)
	{
		PlayerCharacter->PlayerTakeDamage(ProjectileBaseDamage);
		Destroy();
	}

	if(Seeda)
	{
		Seeda->TakeDamage_Seeda(ProjectileBaseDamage);
		Destroy();
	}
	
}
