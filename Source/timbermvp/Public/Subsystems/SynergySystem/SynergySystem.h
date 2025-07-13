// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SynergySystem.generated.h"

struct FStatusEffect;
class UStatusEffectBase;
class UStatusEffectHandlerComponent;

USTRUCT(BlueprintType)
struct FSynergyRules
{
	GENERATED_BODY();

	UPROPERTY()
	FGameplayTagContainer SynergyTags;

	UPROPERTY()
	FGameplayTag EmergentTag;

	FSynergyRules(){};
	
	FSynergyRules(const FGameplayTagContainer& InSynergyTags, const FGameplayTag& InEmergentTag)
		: SynergyTags(InSynergyTags)
		, EmergentTag(InEmergentTag)
	{
	}
};

/**
 * 
 */
UCLASS()
class TIMBERMVP_API USynergySystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	//Rules needed to Generate Emergent Tags.
	TMap<FGameplayTag, TArray<FSynergyRules>> SynergyRules;

	UPROPERTY()
	TMap<FGameplayTag, UStatusEffectBase*> EmergentEffectsMap;
	
	UFUNCTION()
	void ProcessTagForSynergy(FGameplayTag Tag, UStatusEffectHandlerComponent* StatusEffectComponent);

private:
	void CreateSynergyRule(FName Tag1, FName Tag2, FName EmergentTag);

	void SetupEmergentEffectRules();

	TMap<FGameplayTag, UStatusEffectBase*> LoadEmergentEffects();
	
};
