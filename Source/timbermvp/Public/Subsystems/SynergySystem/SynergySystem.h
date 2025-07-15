// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SynergySystem.generated.h"

class UEffectAbilityHandlerBase;
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

	//Runtime Cache of Effect Ability objects per Tag Type.
	UPROPERTY()
	TMap<FGameplayTag, UEffectAbilityHandlerBase*> EffectAbilityMap;

	//Gets or Sets the Tag/Handler (K/V) Pair on the Effect ABility Map && returns the Handler.
	UEffectAbilityHandlerBase* GetOrSetAbilityEffect(FGameplayTag Tag, FStatusEffect StatusEffect);
	
	void HandleEmergentTagChecks(FGameplayTag Tag, UStatusEffectHandlerComponent* StatusEffectComponent);

	void HandleCriticalEffectsLogic(FGameplayTag Tag, UStatusEffectHandlerComponent* StatusEffectComponent, const FStatusEffect& Effect);
	
	UFUNCTION()
	void ProcessTagForSynergy(FGameplayTag Tag, UStatusEffectHandlerComponent* StatusEffectComponent, FStatusEffect& Effect);

private:

	void ExecuteEffectHandlerLogic(UEffectAbilityHandlerBase* EffectHandler);
	
	void CreateSynergyRule(FName Tag1, FName Tag2, FName EmergentTag);

	void SetupEmergentEffectRules();

	//Populates the TMap: EmergentEffectsMap
	TMap<FGameplayTag, UStatusEffectBase*> LoadEmergentEffects();
	
};
