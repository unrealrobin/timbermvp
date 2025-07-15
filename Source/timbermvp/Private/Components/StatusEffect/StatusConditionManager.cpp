// Property of Paracosm.


#include "Components/StatusEffect/StatusConditionManager.h"

#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Data/DataAssets/StatusEffects/StatusEffectDefinition.h"
#include "Types/EffectConditionTypes.h"


// Sets default values for this component's properties
UStatusConditionManager::UStatusConditionManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UStatusConditionManager::BeginPlay()
{
	Super::BeginPlay();
	
	Owner = GetOwner();

	if (!IsValid(Owner))
	{
		UE_LOG(LogTemp, Warning, TEXT("Status Condition Manager: Owner is not set or invalid!"));
	}
}

FEffectConditionContext UStatusConditionManager::GenerateEffectConditionContext(AActor* TargetActor)
{
	FEffectConditionContext EffectConditionContext;

	EffectConditionContext.TargetActor = TargetActor;
	//Stores the Targets Gameplay Tag Container and Active Status Effects TArray.
	EffectConditionContext.TargetStatusEffectHandlerComponent = TargetActor->FindComponentByClass<UStatusEffectHandlerComponent>();
	EffectConditionContext.WorldContext = GetWorld();
	EffectConditionContext.SourceActor = Owner;
	
	return EffectConditionContext;
}

void UStatusConditionManager::ResolveEffect(TArray<UStatusEffectDefinition*> EffectDefinitionsArray, AActor* TargetActor)
{
	if (!IsValid(TargetActor)) return;
	
	FEffectConditionContext Context = GenerateEffectConditionContext(TargetActor);
	
	for (UStatusEffectDefinition* EffectDefinition : EffectDefinitionsArray)
	{
		/*
		 * Evaluates the Status Effects Pair Condition
		 * Status Effect Definition
		 *	- Status Effect
		 *	- Effect Condition <--Rules to check if should apply.
		 */
		
		bool bConditionMet = EffectDefinition->StatusEffectCondition->EvaluateCondition(Context);

		if (bConditionMet)
		{
			if (ATimberEnemyCharacter* Enemy = Cast<ATimberEnemyCharacter>(Context.TargetActor))
			{
				if (!IsValid(Enemy)) return;
				
				//Displays the Critical Synergy Effect Name in Floating Damage UI.
				if (EffectDefinition->StatusEffectAsset->StatusEffect.EffectLevel == EStatusEffectLevel::Critical)
				{
					FName TagName = Enemy->StatusEffectHandler->GetLastNameOfTag(EffectDefinition->StatusEffectAsset->StatusEffect.EffectIdTag);
					Enemy->SpawnEffectNameUI(TagName, EffectDefinition->StatusEffectAsset);
				}
				
				Enemy->StatusEffectHandler->AddStatusEffectToComponent(EffectDefinition->StatusEffectAsset->StatusEffect, Context.SourceActor);
			}
		}
	}
}


