// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Abilities/WeaponAbilityBase.h"
#include "FireBasicProjectile.generated.h"

class ATimberProjectileBase;
/**
 * 
 */
UCLASS(Blueprintable)
class TIMBERMVP_API UFireBasicProjectile : public UWeaponAbilityBase
{
	GENERATED_BODY()

public:
	
	UFireBasicProjectile();
	
	virtual void Execute(FAbilityContext Context) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Details")
	float TimeBetweenProjectiles = 0.5f;

protected:
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<ATimberProjectileBase> ProjectileClass;
	
};
/*
 * Validation Needs.
 * 1. has enough power for this ability.
 *	- need to access this via specific class not generic
 *	2. is the weapon on any cooldowns.
 */