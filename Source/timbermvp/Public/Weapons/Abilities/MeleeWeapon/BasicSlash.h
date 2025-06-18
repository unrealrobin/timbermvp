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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability Data")
	FAbilityContext BasicSlashContext;
	
	virtual void Execute(FAbilityContext Context) override;

	virtual void HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Data")
	UAnimMontage* BasicSlashMontage = nullptr;

	/*Combo Data*/

	//Initialized to -1 (No Section Played Yet)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	int32 SectionIndex = 0;

	//Whether the player attacked again within the CanCombo window.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	bool bComboInitiated = false;

	//The Window for when additional attack inputs will be allowed to make combo attacks.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	bool bComboWindowOpen = false;

	UFUNCTION(BlueprintCallable, Category = "Combo")
	void ResetComboData();
};
