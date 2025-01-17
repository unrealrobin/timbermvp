// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageableEnemy.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageableEnemy : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TIMBERMVP_API IDamageableEnemy
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void TakeDamage(float DamageAmount, AActor* DamageInstigator) = 0;
	virtual void PlayProjectileHitSound(FHitResult HitResult) = 0;
	virtual void PlayMeleeWeaponHitSound(FHitResult HitResult) = 0;
};
