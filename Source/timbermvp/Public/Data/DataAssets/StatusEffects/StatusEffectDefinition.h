﻿// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffectBase.h"
#include "EffectConditions/EffectConditionBase.h"
#include "Engine/DataAsset.h"
#include "StatusEffectDefinition.generated.h"

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UStatusEffectDefinition : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	UStatusEffectBase* StatusEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	UEffectConditionBase* StatusEffectCondition;
};
