// Property of Paracosm.


#include "Components/StatusEffect/StatusConditionManager.h"

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

	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Status Condition Manager: Owner is not set or invalid!"));
	}
	
}

//TODO:: This also need an Input of Status Effect Definition which has a StatusEffect and EffectCondition DA stored on it.
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
	FEffectConditionContext Context = GenerateEffectConditionContext(TargetActor);

	//loops over each of the traps effect. Each effect is checked everytime.
	//TODO:: Need to check the order in which these get called, maybe the best way is reverse order.
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
			Context.TargetStatusEffectHandlerComponent->AddStatusEffectToComponent(EffectDefinition->StatusEffect->StatusEffect);
		}
	}

}


