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
	UPROPERTY()
	TObjectPtr<AActor> TargetActorCache = nullptr;
	
	virtual void HandleVFX(AActor* TargetActor, const FStatusEffect& StatusEffect);
	virtual void HandleSFX(AActor* TargetActor, const FStatusEffect& StatusEffect);

	TArray<AActor*> GetActorsInRadius_IncludeTarget(AActor* SourceActor, float Radius);
	void RemoveEmergentTagFromActor(AActor* TargetActor, FGameplayTag Tag);
	TArray<AActor*> SortActorsByDistance(TArray<AActor*> TargetActors, bool bReturnAllActors = true, int HowManyToReturn = 0);
	
};
