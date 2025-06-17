// Property of Paracosm.


#include "Weapons/Abilities/MeleeWeapon/BasicSlash.h"

#include "Weapons/TimberWeaponMeleeBase.h"

UBasicSlash::UBasicSlash()
{
	ABILITY_NAME = "BasicSlash";
	ValidationType = EAbilityValidation::NoResourceCost;
	WeaponAbilityType = EForWeaponType::MeleeWeapon;
	InputRequirement = EAbilityInputRequirement::Pressed;
	bNeedsProjectileData = false;
}

void UBasicSlash::Execute(FAbilityContext Context)
{
	//Weapon Holds Animation
	ATimberWeaponMeleeBase* MeleeWeapon = Cast<ATimberWeaponMeleeBase>(Context.WeaponInstance);
	if (MeleeWeapon)
	{
		//Anim Montage will reset this with Notifies
		Context.CombatComponent->bCanMeleeAttack = true;
		
		/* THis will only work for Kip - Utilizes references on the Weapon directly for Kip.*/
		//MeleeWeapon->HandlePlayAttackMontage();

		//Combo window is open and available, and the combo hasn't been initiated yet.
		if (MeleeWeapon->bComboWindowOpen && !MeleeWeapon->bComboInitiated)
		{
			//Tells melee weapon to initiate combo
			MeleeWeapon->bComboInitiated = true;
			
			UE_LOG(LogTemp, Warning, TEXT("Combo Initiated"));

			//Only used to Increment Combo Montage Section Index.
			MeleeWeapon->HandlePlayComboAttackMontage();
		}
		else if (MeleeWeapon->bComboWindowOpen && MeleeWeapon->bComboInitiated)
		{
			//We don't want successive input pressed while combo is already initiated.
			UE_LOG(LogTemp, Warning, TEXT("Combo Already Initiated. Early Return from Input."));
			return;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Initial Combo Attack Play Animation."));
			MeleeWeapon->HandlePlayComboAttackMontage();
		}
		
	}
}
