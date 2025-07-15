// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "EffectAbilityHandlerBase.generated.h"

class UStatusEffectBase;
struct FStatusEffect;
/**
 * 
 */
UCLASS()
class TIMBERMVP_API UEffectAbilityHandlerBase : public UObject
{
	GENERATED_BODY()

public:

	UEffectAbilityHandlerBase();
	
	UFUNCTION()
	virtual void ExecuteEffect(AActor* TargetActor, const FStatusEffect& StatusEffect);

protected:

	/* Removes some @Tag
	 * Level 3 Effects - Removes the Causing Emergent Tag
	 */
	void RemoveEmergentTagFromActor(AActor* TargetActor, FGameplayTag Tag);

	
	TArray<AActor*> GetActorsInRadius(AActor* SourceActor, float Radius);
};
