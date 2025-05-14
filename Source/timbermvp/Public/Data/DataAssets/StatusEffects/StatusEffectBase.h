// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "StatusEffectBase.generated.h"

USTRUCT(BlueprintType)
struct FStatusEffect
{
	GENERATED_BODY()

	// --- Required: Identity & Behavior ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Tags")
	FGameplayTag EffectIdTag; // e.g., Status.Wet, Status.Corrosive

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Tags")
	FGameplayTagContainer MetaTagContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Tags")
	FGameplayTagContainer TypeTagContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 5.f; // How long the status lasts
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TickInterval = 0.f; // 0 = no ticking

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamagePerTick = 0.f; // Applied each tick
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialDamage = 0.f; // Dealt once on application
    
	int CurrentStacks = 1;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxStacks = 5;
	
	// --- Runtime-Only ---
	float TimeRemaining = 0.f;
	float TickAccumulator = 0.f;
};

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UStatusEffectBase : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FStatusEffect StatusEffect;
	
};
