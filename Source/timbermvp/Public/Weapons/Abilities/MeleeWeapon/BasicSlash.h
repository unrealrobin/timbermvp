// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Abilities/WeaponAbilityBase.h"
#include "BasicSlash.generated.h"

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UBasicSlash : public UWeaponAbilityBase
{
	GENERATED_BODY()

public:
	
	UBasicSlash();
	
	virtual void Execute(FAbilityContext Context) override;
};
