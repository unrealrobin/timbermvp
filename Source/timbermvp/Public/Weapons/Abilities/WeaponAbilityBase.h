// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/CombatComponent.h"
#include "UObject/Object.h"
#include "WeaponAbilityBase.generated.h"

UENUM(BlueprintType)
enum class EAbilityValidation: uint8
{
	AlwaysCastable UMETA(DisplayName = "AlwaysCastable"),
	RequiresPower UMETA(DisplayName = "RequiresPower"),
	RequiresAmmo UMETA(DisplayName = "RequiresAmmo"),
	Default UMETA(DisplayName = "Default"),
};

UENUM(BlueprintType)
enum class EForWeaponType: uint8
{
	RangedWeapon UMETA(DisplayName = "RangedWeapon"),
	MeleeWeapon UMETA(DisplayName = "MeleeWeapon"),
	Default UMETA(DisplayName = "Default"),
};


/*struct FAbilityContext;*/
/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class TIMBERMVP_API UWeaponAbilityBase : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Details", meta=(DisplayName="Ability Name"))
	FName ABILITY_NAME = "NAME_NOT_SET";

	/*How the combat Controller should validate this ability*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Details")
	EAbilityValidation ValidationType = EAbilityValidation::Default;

	/*The type of weapon this ability is for.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Details")
	EForWeaponType WeaponAbilityType = EForWeaponType::Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Details")
	float PowerCost = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Details")
	bool bNeedsProjectileData = false;
	
	/*
	 * All Subclasses must Implement this function.
	 * This is what gets called by the Combat Component to initiate this Ability.
	 */
	virtual void Execute(FAbilityContext Context) PURE_VIRTUAL(UWeaponAbilityBase::Execute,);
	
};
