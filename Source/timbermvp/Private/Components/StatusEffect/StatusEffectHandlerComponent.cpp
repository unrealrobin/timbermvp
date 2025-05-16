// Property of Paracosm.


#include "Components/StatusEffect/StatusEffectHandlerComponent.h"
#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Data/DataAssets/StatusEffects/StatusEffectBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UStatusEffectHandlerComponent::UStatusEffectHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStatusEffectHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningEnemyCharacter = Cast<ATimberEnemyCharacter>(GetOwner());
	if (!OwningEnemyCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Status Effect Handler Component is not attached to a Timber Enemy Character!"));
	}
	
}

void UStatusEffectHandlerComponent::HandleDotEffects(FStatusEffect& StatusEffect, float DeltaTime)
{
	//Handle Any DOT Tags
	for (FGameplayTag Tag : StatusEffect.TypeTagContainer)
	{
		if (Tag == FGameplayTag::RequestGameplayTag("BuildableEffects.Type.DOT"))
		{
			StatusEffect.TickAccumulator += DeltaTime;
			if (StatusEffect.TickAccumulator >= StatusEffect.TickInterval)
			{
				//Stacks multiply the Damage per Tick.
				float TotalDamage = StatusEffect.DamagePerTick * StatusEffect.CurrentStacks;
				OwningEnemyCharacter->TakeDamage(TotalDamage, nullptr);
				//UE_LOG(LogTemp, Warning, TEXT("Applied Total Damage Dot: %f"), TotalDamage);
				//Resetting the Tick Accumulator
				StatusEffect.TickAccumulator = 0;
			}
		}
	}
}

void UStatusEffectHandlerComponent::HandleMetaRemovals(FStatusEffect& StatusEffect)
{
	for (FGameplayTag Tag : StatusEffect.TypeTagContainer)
	{
		if (Tag == FGameplayTag::RequestGameplayTag("BuildableEffects.Meta.Removes.Slow"))
		{
			//Go through the Status Effects - Check for Type Tags with Slow
			for (FStatusEffect& CheckedStatusEffect : ActiveStatusEffects)
			{
				FGameplayTag TagToCompare = FGameplayTag::RequestGameplayTag("BuildableEffects.Type.Slow");
				if (CheckedStatusEffect.TypeTagContainer.HasTag(TagToCompare))
				{
					//UE_LOG(LogTemp, Warning, TEXT("Removed a Slow Effect."));
					StagedForRemoval.Add(CheckedStatusEffect);
				}
			}
			//Stage for removal
		}
	}
}

void UStatusEffectHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (FStatusEffect& StatusEffect : ActiveStatusEffects)
	{
		/*If the effect still has time remaining*/
		if (StatusEffect.TimeRemaining > 0.f)
		{
			//Incrementing
			StatusEffect.TimeRemaining -= DeltaTime;

			HandleDotEffects(StatusEffect, DeltaTime);

			HandleMetaRemovals(StatusEffect);
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Removing Effect."));
			StagedForRemoval.Add(StatusEffect);
		}
	}

	//Handles Removal of all Staged Removal Effects
	if (StagedForRemoval.Num() > 0)
	{
		RemoveMultipleEffectsFromComponent(StagedForRemoval);
	}
}

void UStatusEffectHandlerComponent::HandleIsStackableEffect(FStatusEffect* Effect)
{
	if (Effect->TypeTagContainer.HasTag(FGameplayTag::RequestGameplayTag("BuildableEffects.Type.Stackable")))
	{
		//Look through the Active Status Effects TArray for the Effect with the Effect Id Tag for this effect.
		//FStatusEffect* StackableEffect = FindEffectByIdTag(Effect->EffectIdTag);

		if (Effect && Effect->CurrentStacks < Effect->MaxStacks)
		{
			//Increment the stacks if possible.
			Effect->CurrentStacks++;
			ResetEffectDuration(*Effect);
		}
	}
}

void UStatusEffectHandlerComponent::HandleSlowTags(const FStatusEffect& Effect, float MaxWalkSpeedBaseMultiplier)
{
	/*Handle Slow Tags*/
	for (FGameplayTag Tag : Effect.TypeTagContainer)
	{
		if (Tag == FGameplayTag::RequestGameplayTag("BuildableEffects.Type.Slow"))
		{
			OwningEnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = OwningEnemyCharacter->MaxWalkSpeedBase *  MaxWalkSpeedBaseMultiplier; 
			//UE_LOG(LogTemp, Warning, TEXT("Slowed Enemy: %s"), *OwningEnemyCharacter->GetName());
		}
	}
}

void UStatusEffectHandlerComponent::HandleEffectInitialDamage(FStatusEffect& Effect)
{
	if (Effect.InitialDamage > 0.f)
	{
		OwningEnemyCharacter->TakeDamage(Effect.InitialDamage, nullptr);
	}
}

void UStatusEffectHandlerComponent::AddStatusEffectToComponent(FStatusEffect& Effect)
{
	//Does this unique id already exist in the container?
	bool bTagExists = CheckIfTagAlreadyExists(Effect.EffectIdTag);

	if (!bTagExists) //Doesn't already exist
	{
		StatusEffectIdTagContainer.AddTag(Effect.EffectIdTag);
		ActiveStatusEffects.Add(Effect);

		//Initializing TimeRemaining to Start at Effect Duration
		Effect.TimeRemaining = Effect.Duration;
		
		/*Handle Any Initial Damage*/
		HandleEffectInitialDamage(Effect);
		
		HandleSlowTags(Effect, 0.5f); //50% slower
	}
	else
	{
		//If the ID Already existing in our array, we need to get the existing version of the effect. And add a stack. Then we need to reset the duration
		//because its basically a new iteration of the effect at a higher stack.
		
		FStatusEffect* ExisitingStatusEffect = FindEffectByIdTag(Effect.EffectIdTag);
		if (ExisitingStatusEffect)
		{
			HandleIsStackableEffect(ExisitingStatusEffect);
		}
	}
}

bool UStatusEffectHandlerComponent::CheckIfTagAlreadyExists(FGameplayTag TagToCheck)
{
	//It is a unique tag and should be exact.
	 return StatusEffectIdTagContainer.HasTagExact(TagToCheck);
}

void UStatusEffectHandlerComponent::RemoveEffectFromComponent(const FStatusEffect& Effect)
{
	//Check the array for this effect and Remove it.

	//Remove Any Movement Speed Encumbrances applied during effect.
	HandleSlowTags(Effect, 1.0f); //Back to normal speed
	
	ActiveStatusEffects.RemoveAll([Effect](const FStatusEffect& ActiveEffect)
	{
		return ActiveEffect.EffectIdTag == Effect.EffectIdTag;
	});
	
	//Remove The Unique Id Tag from the Tag Container.
	StatusEffectIdTagContainer.RemoveTag(Effect.EffectIdTag);
	
}

void UStatusEffectHandlerComponent::RemoveMultipleEffectsFromComponent(TArray<FStatusEffect>& Effects)
{
	for (FStatusEffect& StatusEffect : Effects)
	{
		RemoveEffectFromComponent(StatusEffect);
	}

	StagedForRemoval.Empty();
}

void UStatusEffectHandlerComponent::ResetEffectDuration(FStatusEffect& Effect)
{
	//Resetting the Duration of the effect. Typically used on the application of a new stack.
	Effect.TimeRemaining = Effect.Duration;
}

FStatusEffect* UStatusEffectHandlerComponent::FindEffectByIdTag(const FGameplayTag& Tag)
{
	return ActiveStatusEffects.FindByPredicate([&Tag]( FStatusEffect& Effect)
	{
		return Effect.EffectIdTag == Tag;
	});
}





