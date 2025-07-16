// Property of Paracosm.


#include "Synergy/EffectLogic/DerivedHandlers/BlizzardAbilityHandler.h"

#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Components/StatusEffect/StatusEffectHandlerComponent.h"

UBlizzardAbilityHandler::UBlizzardAbilityHandler()
{
}

void UBlizzardAbilityHandler::ExecuteEffect(AActor* TargetActor,
                                            const FStatusEffect& StatusEffect)
{
	Super::ExecuteEffect(TargetActor, StatusEffect);

	UE_LOG(LogTemp, Warning, TEXT("Blizzard Ability Executed."));

	TArray<AActor*> ActorsInRadius = GetActorsInRadius_IncludeTarget(TargetActor, 1000.0f);
	for (AActor* ActorInRadius : ActorsInRadius)
	{
		//Apply Freeze Effect.
		if (ATimberEnemyCharacter* Enemy = Cast<ATimberEnemyCharacter>(ActorInRadius))
		{
			if (IsValid(Enemy))
			{
				Enemy->StatusEffectHandler->AddStatusEffectToComponent(StatusEffect.SecondaryEffect->StatusEffect);
			}
		}
	}
}
