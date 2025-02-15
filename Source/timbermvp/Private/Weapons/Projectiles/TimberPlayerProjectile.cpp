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
		//UE_LOG(LogTemp, Warning, TEXT("Overlapped: %ls"), *OverlappedComponent->GetFullName());
		//Play the IDamageableEnemy's TakeDamage function. Interface.
		HitEnemy->PlayProjectileHitSound(SweepResult);
		// Print the owner of the weapon
		UE_LOG(LogTemp, Warning, TEXT("Projectile Owner: %s"), *GetOwner()->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Projectile's Owner's Owner: %s"), *GetOwner()->GetOwner()->GetName());

		ATimberPlayableCharacter* OwningCharacter = Cast<ATimberPlayableCharacter>(GetOwner()->GetOwner());
		if (OwningCharacter)
		{
			HitEnemy->TakeDamage(CalculateOutputDamage(OwningCharacter), GetOwner()->GetOwner());
		}
		// The Projectiles Owner is the Weapon and the Weapons Owner is the Player Character. The Player Character needs to be passed to Take Damage to calculate the Aggro condition.

		//Destroys the projectile on hitting an enemy that may take damage from this projectile.
		Destroy();
	}
	else
	{
		Destroy();
	}
}

float ATimberPlayerProjectile::CalculateOutputDamage(AActor* PlayerActor)
{
	/*Projectile Damage can be modified by the Players Modifier && The Weapons Modifier
	 * This will allow effects to either Modify only Ranged Weapons or Player and Ranged Weapons or just the Player.
	 */
	ATimberPlayableCharacter* PlayerCharacter = Cast<ATimberPlayableCharacter>(PlayerActor);
	ATimberWeaponRangedBase* ProjectileOwningRangedWeapon = Cast<ATimberWeaponRangedBase>(GetOwner());
	if (PlayerCharacter)
	{
		float TotalDamage = ProjectileBaseDamage * ProjectileOwningRangedWeapon->DamageModifierValue  * 
		PlayerCharacter->DamageModifierValue;
		
		UE_LOG(LogTemp, Warning, TEXT("BaseProjectileDamage: %f. WeaponModifierValue: %f. PlayerModifierValue: %f.  Total Damage: %f"),
			ProjectileBaseDamage,
			ProjectileOwningRangedWeapon->DamageModifierValue,
			PlayerCharacter->DamageModifierValue,
			TotalDamage);
		

		return TotalDamage;
	}

	return ProjectileBaseDamage;
}
