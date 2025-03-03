// Property of Paracosm Industries. Dont use my shit.
#include "Weapons/Projectiles/TimberPlayerProjectile.h"

#include "Components/CapsuleComponent.h"
#include "Interfaces/DamageableEnemy.h"
#include "Weapons/TimberWeaponRangedBase.h"

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
		CapsuleComponent->OnComponentHit.AddDynamic(this, &ATimberPlayerProjectile::HandleBlocked);
	}
}

// Called when the game starts or when spawned
void ATimberPlayerProjectile::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("Projectile Spawned."));
}

// Called every frame
void ATimberPlayerProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATimberPlayerProjectile::HandleBlocked(
	UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	const FHitResult& Hit)
{
	//if the projectile is blocked by a wall or other object, destroy the projectile. This is only for the Players 
	// Projectile. Different collisions for enemy projectile.
	Destroy();
	//UE_LOG(LogTemp, Warning, TEXT("PlayerProjectile - Player Projectile has been Blocked and Destroyed."))
}

void ATimberPlayerProjectile::HandleOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Projectile Owner: %s"), *GetOwner()->GetName());
	IDamageableEnemy* HitEnemy = Cast<IDamageableEnemy>(OtherActor);

	if (HitEnemy)
	{
		//UE_LOG(LogTemp, Warning, TEXT("HitEnemy Valid."));
		//Play the IDamageableEnemy's TakeDamage function. Interface.
		HitEnemy->PlayProjectileHitSound(SweepResult);

		//Weapon Owns Projectile, Player Owns Weapon.
		if (PlayerProjectileOwner)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Owning Weapon is Valid."));
			HitEnemy->TakeDamage(CalculateOutputDamage(Cast<ATimberWeaponRangedBase>(GetOwner())), PlayerProjectileOwner);
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Owning Weapon Not Valid."));
		}
		
		//Destroys the projectile on hitting an enemy that may take damage from this projectile.
		//UE_LOG(LogTemp, Warning, TEXT("Timer Started to Handle Destruction."));
		FTimerHandle OnHandleDestroy;
		//GetWorld()->GetTimerManager().SetTimer(OnHandleDestroy, this, &ATimberPlayerProjectile::HandleDestroy, 0.2f, false);
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ATimberPlayerProjectile::HandleDestroy);
	}
	
	
}

float ATimberPlayerProjectile::CalculateOutputDamage(ATimberWeaponRangedBase* Weapon)
{
	/*Projectile Damage can be modified by the Players Modifier && The Weapons Modifier
	 * This will allow effects to either Modify only Ranged Weapons or Player and Ranged Weapons or just the Player.
	 */
	if (PlayerProjectileOwner)
	{
		float TotalDamage = ProjectileBaseDamage * (Weapon->DamageModifierValue) * (PlayerProjectileOwner->DamageModifierValue);
		
		/*UE_LOG(LogTemp, Warning, TEXT("BaseProjectileDamage: %f. WeaponModifierValue: %f. PlayerModifierValue: %f.  Total Damage: %f"),
			ProjectileBaseDamage,
			Weapon->DamageModifierValue,
			PlayerProjectileOwner->DamageModifierValue,
			TotalDamage);*/
		
		return TotalDamage;
	}
	return ProjectileBaseDamage;
}

void ATimberPlayerProjectile::HandleDestroy()
{
	//UE_LOG(LogTemp, Warning, TEXT("Projectile Destroyed!"));
	Destroy();
}