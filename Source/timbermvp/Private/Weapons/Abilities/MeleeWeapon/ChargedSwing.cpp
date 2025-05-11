// Property of Paracosm.


#include "Weapons/Abilities/MeleeWeapon/ChargedSwing.h"

#include "Weapons/TimberWeaponMeleeBase.h"

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

	AbilityContext = Context;

	Context.CombatComponent->PlayCharacterAnimationMontage(ChargedSwingMontage, "WindUp", .5, true);
	CurrentMontageStage = EMontageStage::WindUp;
}

void UChargedSwing::Execute_Completed(FAbilityContext Context)
{
	if (NiagaraEffect)
	{
		if (ATimberWeaponMeleeBase* MeleeWeapon = Cast<ATimberWeaponMeleeBase>(Context.WeaponInstance))
		{
			//Spawn the effect at the location of the weapon.
			PlayNiagaraEffectAtLocation(NiagaraEffect, MeleeWeapon->NiagaraEffectSpawnLocation->GetComponentLocation(), Context.Instigator->GetActorForwardVector().Rotation());
		}
	}
	Context.CombatComponent->PlayCharacterAnimationMontage(ChargedSwingMontage, "ChargedAttack", 1.0f);
	CurrentMontageStage = EMontageStage::Final;
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
		Execute_Completed(Context);
	}
	else
	{
		HandleIncompleteAbility(Context);
	}
}

void UChargedSwing::HandleIncompleteAbility(FAbilityContext Context)
{
	UE_LOG(LogTemp, Warning, TEXT("Player Released Before Full Charge Met."));
	//Stop Any Anim Montage
	if (Context.CombatComponent)
	{
		Context.CombatComponent->StopCharacterAnimationMontage(ChargedSwingMontage, .33f);
	}
	HandleCleanup(Context);
}

void UChargedSwing::HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	//Interrupted only true if the animation was interrupted.If finished properly, it will be false.
	//We only want to force a new montage if the previous montage was finished.
	if (bInterrupted) return;
	
	switch (CurrentMontageStage)
	{
	case EMontageStage::None:
		break;
	case EMontageStage::WindUp:
		//Handle StartLoop
		if (AbilityContext.CombatComponent)
		{
			//AbilityContext.CombatComponent->PlayCharacterAnimationMontage(ChargedSwingMontage, "ChargeLoadLoop", .01f, true);
			//CurrentMontageStage = EMontageStage::Loop;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not access Ability Context Variable to start Loop Anim Section."));
		}
		break;
	case EMontageStage::Loop:
		//ContinueToPlayLoop
		if (AbilityContext.CombatComponent)
		{
			//AbilityContext.CombatComponent->PlayCharacterAnimationMontage(ChargedSwingMontage, "ChargeLoadLoop", .5f, true);
		}
		break;
	case EMontageStage::Final:
		//Do Nothing
		break;
	default:
		break;
	}
}
