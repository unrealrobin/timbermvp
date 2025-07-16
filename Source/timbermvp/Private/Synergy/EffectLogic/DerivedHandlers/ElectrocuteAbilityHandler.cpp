// Property of Paracosm.


#include "Synergy/EffectLogic/DerivedHandlers/ElectrocuteAbilityHandler.h"

UElectrocuteAbilityHandler::UElectrocuteAbilityHandler()
{
}

void UElectrocuteAbilityHandler::ExecuteEffect(AActor* TargetActor,
                                               const FStatusEffect& StatusEffect)
{
	Super::ExecuteEffect(TargetActor, StatusEffect);


	TArray<AActor*> ActorsInRadius = GetActorsInRadius_IncludeTarget(TargetActor, 500.0f);
	
	UE_LOG(LogTemp, Warning, TEXT("Electrocute Ability Executed."));
}
