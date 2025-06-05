// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "StatusEffectHandlerComponent.generated.h"


class ATimberEnemyCharacter;
struct FStatusEffect;
class UStatusEffectBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TIMBERMVP_API UStatusEffectHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatusEffectHandlerComponent();
	
	void HandleEffectInitialDamage(FStatusEffect& Effect);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status Effects")
	TArray<FStatusEffect> ActiveStatusEffects;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect Tags")
	FGameplayTagContainer StatusEffectIdTagContainer;

	void AddStatusEffectToComponent(FStatusEffect& Effect);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void HandleDotEffects(FStatusEffect& StatusEffect, float DeltaTime);
	
	void HandleMetaPerpetualRemovals(FStatusEffect& StatusEffect);
	void HandleMetaInitialRemovals(FStatusEffect& StatusEffect);

	/*
	 * Checks if the Tag (EffectIdTag) already exists in the StatusEffectIdTagContainer.
	 */
	bool CheckIfTagAlreadyExists(FGameplayTag TagToCheck);

	/*
	 * Checks if the Effect is stackable and Increments the stacks of the effect if possible.
	 */
	void HandleIsStackableEffect(FStatusEffect* Effect);

	/*
	 * Handle Slow Tags
	 */
	void HandleSlowTags(const FStatusEffect& Effect, float MaxWalkSpeedBaseMultiplier);

	/* Update Status Effect Bar on Characters*/
	void AddEffectToStatusEffectBar(FGameplayTag EffectTag);
	void RemoveEffectFromStatusEffectBar(FGameplayTag EffectTag);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void RemoveEffectFromComponent(const FStatusEffect& Effect);
	void RemoveMultipleEffectsFromComponent(TArray<FStatusEffect>& Effects);
	void ResetEffectDuration(FStatusEffect& Effect);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StatusEffects")
	ATimberEnemyCharacter* OwningEnemyCharacter = nullptr;

private:
	/*
	 * Why do we Stage For Removal?
	 * We don't want to modify the ActiveStatusEffects array while iterating through it every tick. So we loop every status effect,
	 * and the last part of logic in tick clears any staged status effects for destruction.
	 */
	TArray<FStatusEffect> StagedForRemoval;

	FStatusEffect* FindEffectByIdTag(const FGameplayTag& Tag);
};
