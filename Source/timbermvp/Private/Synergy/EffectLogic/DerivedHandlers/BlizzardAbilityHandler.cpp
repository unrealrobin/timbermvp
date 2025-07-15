// Property of Paracosm.


#include "Synergy/EffectLogic/DerivedHandlers/BlizzardAbilityHandler.h"

UBlizzardAbilityHandler::UBlizzardAbilityHandler()
{
}

void UBlizzardAbilityHandler::ExecuteEffect(AActor* TargetActor,
                                            const FStatusEffect& StatusEffect)
{
	Super::ExecuteEffect(TargetActor, StatusEffect);

	UE_LOG(LogTemp, Warning, TEXT("Blizzard Ability Executed."));
}
