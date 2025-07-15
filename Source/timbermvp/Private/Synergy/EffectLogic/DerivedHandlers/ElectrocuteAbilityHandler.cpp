// Property of Paracosm.


#include "Synergy/EffectLogic/DerivedHandlers/ElectrocuteAbilityHandler.h"

UElectrocuteAbilityHandler::UElectrocuteAbilityHandler()
{
}

void UElectrocuteAbilityHandler::ExecuteEffect(AActor* TargetActor,
                                               const FStatusEffect& StatusEffect)
{
	Super::ExecuteEffect(TargetActor, StatusEffect);

	UE_LOG(LogTemp, Warning, TEXT("Electrocute Ability Executed."));
}
