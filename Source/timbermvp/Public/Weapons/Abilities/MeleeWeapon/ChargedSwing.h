// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Abilities/WeaponAbilityBase.h"
#include "ChargedSwing.generated.h"

UENUM(Blueprintable)
enum class EMontageStage: uint8
{
	None,
	WindUp,
	Loop,
	Final
};

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UChargedSwing : public UWeaponAbilityBase
{
	GENERATED_BODY()
	
public:
	
	UChargedSwing();

	virtual void Execute(FAbilityContext Context) override;
	virtual void Execute_Completed(FAbilityContext Context) override;
	virtual void Execute_Cancelled(FAbilityContext Context) override;

	virtual void Execute_Triggered(FAbilityContext Context) override;
	
	void HandleIncompleteAbility(FAbilityContext Context);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability Data")
	float ChargeTimer = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Data")
	float FullChargeTimer = 2.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability Data")
	EMontageStage CurrentMontageStage = EMontageStage::None;

	virtual void HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Data")
	UAnimMontage* ChargedSwingMontage = nullptr;

private:
	UPROPERTY()
	FAbilityContext AbilityContext;
	
	
};
