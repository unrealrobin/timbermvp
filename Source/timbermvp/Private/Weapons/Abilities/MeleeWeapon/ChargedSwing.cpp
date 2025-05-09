// Property of Paracosm.


#include "Weapons/Abilities/MeleeWeapon/ChargedSwing.h"

UChargedSwing::UChargedSwing()
{
	ABILITY_NAME = "ChargedSwing";
	ValidationType = EAbilityValidation::RequiresPower;
	WeaponAbilityType = EForWeaponType::MeleeWeapon;
	bNeedsProjectileData = false;
	PowerCost = 30.0f;
}

void UChargedSwing::Execute(FAbilityContext Context)
{
	//Storing the Time since the Ability was pressed.
	ChargeTimer = GetWorld()->GetTimeSeconds();
	
	
}

void UChargedSwing::Execute_Completed(FAbilityContext Context)
{
	
	HandleCleanup(Context);
}

void UChargedSwing::Execute_Triggered(FAbilityContext Context)
{
	//UE_LOG(LogTemp, Warning, TEXT("Input Reached Max Hold Activation Threshold. Executing Attack."));
	
	HandleCleanup(Context);
}

void UChargedSwing::Execute_Cancelled(FAbilityContext Context)
{
	float CancelTime = GetWorld()->GetTimeSeconds();
	ChargeTimer = CancelTime - ChargeTimer;
	if (ChargeTimer > FullChargeTimer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon Fully Charged - Executing Attack."))
	}
	else
	{
		HandleIncompleteAbility(Context);
	}

	HandleCleanup(Context);
}

void UChargedSwing::HandleIncompleteAbility(FAbilityContext Context)
{
	UE_LOG(LogTemp, Warning, TEXT("Player Released Before Full Charge Met."));
}
