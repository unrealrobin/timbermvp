// Property of Paracosm.


#include "Weapons/Abilities/MeleeWeapon/BasicSlash.h"

#include "Weapons/TimberWeaponMeleeBase.h"

UBasicSlash::UBasicSlash()
{
	ABILITY_NAME = "BasicSlash";
	ValidationType = EAbilityValidation::NoResourceCost;
	WeaponAbilityType = EForWeaponType::MeleeWeapon;
	bNeedsProjectileData = false;
}

void UBasicSlash::Execute(FAbilityContext Context)
{
	//TODO:: Must Ensure that only SKM's with the correct Animations can use this ability. Calls a specific animation set on the Weapon.
	//Weapon Holds Animation
	ATimberWeaponMeleeBase* MeleeWeapon = Cast<ATimberWeaponMeleeBase>(Context.WeaponInstance);
	if (MeleeWeapon)
	{
		//Anim Montage will reset this with Notifies
		Context.CombatComponent->bCanMeleeAttack = false;
		/* THis will only work for Kip - Utilizes references on the Weapon directly for Kip.*/
		MeleeWeapon->HandlePlayAttackMontage();
	}
}
