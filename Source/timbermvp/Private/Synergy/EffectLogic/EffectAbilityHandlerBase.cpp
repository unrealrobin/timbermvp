// Property of Paracosm.


#include "Synergy/EffectLogic/EffectAbilityHandlerBase.h"

#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Components/StatusEffect/StatusEffectHandlerComponent.h"
#include "Data/DataAssets/StatusEffects/StatusEffectBase.h"

UEffectAbilityHandlerBase::UEffectAbilityHandlerBase()
{
}

void UEffectAbilityHandlerBase::ExecuteEffect(AActor* TargetActor,
                                              const FStatusEffect& StatusEffect)
{
	if (StatusEffect.TagsToRemove.Num() > 0)
	{
		FGameplayTag TagToRemove = StatusEffect.TagsToRemove.GetByIndex(0);
		RemoveEmergentTagFromActor(TargetActor, TagToRemove);
	}
	
	//Custom Logic here for Each Unique Effect with Abilties.
}

void UEffectAbilityHandlerBase::RemoveEmergentTagFromActor(AActor* TargetActor, FGameplayTag Tag)
{
	if (IsValid(TargetActor))
	{
		ATimberEnemyCharacter* Enemy = Cast<ATimberEnemyCharacter>(TargetActor);
		if (IsValid(Enemy))
		{
			Enemy->StatusEffectHandler->RemoveEmergentTag(Tag);
		}
	}
}

TArray<AActor*> UEffectAbilityHandlerBase::GetActorsInRadius(AActor* SourceActor, float Radius)
{
	TArray<AActor*> ActorsInRadius;

	if (IsValid(SourceActor) && Radius > 0.0f)
	{
		
	}

	return ActorsInRadius;
}
