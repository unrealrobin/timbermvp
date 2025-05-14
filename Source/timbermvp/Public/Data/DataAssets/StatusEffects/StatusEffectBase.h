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

	//Duration of the effect before removal.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 5.f; // How long the status lasts

	//Ticks every Tick Interval Seconds. e.g., 1.0f Ticks ever 1 Second.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TickInterval = 0.f; 

	//Damage Applied at every Tick Interval
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamagePerTick = 0.f; // Applied each tick

	//Any Initial Damage Caused by Effect.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialDamage = 0.f; // Dealt once on application

	//Always should be <= Max Stacks
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int CurrentStacks = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxStacks = 5;
	
	// --- Runtime-Only ---

	// Time Remaining before removal.
	float TimeRemaining = 0.f;

	//Time tracker between ticks
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
