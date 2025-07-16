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
	if (IsValid(TargetActor))
	{
		TargetActorCache = TargetActor;
	}
	
	if (StatusEffect.TagsToRemove.Num() > 0)
	{
		FGameplayTag TagToRemove = StatusEffect.TagsToRemove.GetByIndex(0);
		RemoveEmergentTagFromActor(TargetActor, TagToRemove);
	}
	
	//Custom Logic here for Each Unique Effect with Abilties.
}

void UEffectAbilityHandlerBase::HandleSFX(AActor* TargetActor, const FStatusEffect& StatusEffect)
{
	//Play Any Sounds Required.
}

void UEffectAbilityHandlerBase::HandleVFX(AActor* TargetActor, const FStatusEffect& StatusEffect)
{
	//Active any Niagara Systems that are required.
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

TArray<AActor*> UEffectAbilityHandlerBase::SortActorsByDistance(TArray<AActor*> TargetActors, bool bReturnAllActors,int HowManyToReturn)
{
	//Target Actor Cache is a Stored Ref to the initial Actor the rest of this should be based on.
	//Target Actors is already retrieved.
	if (TargetActors.Num() > 1 && IsValid(TargetActorCache))
	{
		TargetActors.Sort([this](const AActor* ActorA, const AActor* ActorB)
		{
			return FVector::DistSquared(TargetActorCache->GetActorLocation(), ActorA->GetActorLocation() ) > FVector::DistSquared(TargetActorCache->GetActorLocation(), ActorB->GetActorLocation());
		});
	}

	if (!bReturnAllActors && HowManyToReturn >= 1)
	{
		if (TargetActors.IsValidIndex(HowManyToReturn - 1))
		{
			TargetActors.RemoveAt(HowManyToReturn - 1, TargetActors.Num() - HowManyToReturn, false);
		}
	}

	return TargetActors;

}

TArray<AActor*> UEffectAbilityHandlerBase::GetActorsInRadius_IncludeTarget(AActor* SourceActor, float Radius)
{
	TArray<AActor*> ActorsInRadius;

	//If the found actors Z location difference is larger than this, don't count it.
	float VerticalOffsetAmount = 75.0f;
	
	if (IsValid(SourceActor) && Radius > 0.0f)
	{
		TArray<FOverlapResult> OverlapResults;
		FCollisionQueryParams QueryParams;
		bool bHits = SourceActor->GetWorld()->OverlapMultiByChannel(OverlapResults, SourceActor->GetActorLocation(), FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(Radius), QueryParams);
		if (bHits && OverlapResults.Num() > 0)
		{
			for (FOverlapResult& OverlapResult : OverlapResults)
			{
				//Checking Z Diffrence of Hit Actor and Source Actor
				if (FMath::Abs(SourceActor->GetActorLocation().Z - OverlapResult.GetActor()->GetActorLocation().Z) < VerticalOffsetAmount)
				{
					ActorsInRadius.Add(OverlapResult.GetActor());
				}
			}
		}
	}
	return ActorsInRadius;
}
