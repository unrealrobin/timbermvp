// Property of Paracosm.


#include "Components/StatusEffect/StatusEffectHandlerComponent.h"
#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Data/DataAssets/StatusEffects/StatusEffectBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UStatusEffectHandlerComponent::UStatusEffectHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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
		/*UE_LOG(LogTemp, Warning, TEXT("Effect Tag Added to Status Effect ID Container."));
		UE_LOG(LogTemp, Warning, TEXT("StatusEffectIdTagContainer: %d"), StatusEffectIdTagContainer.Num());
		UE_LOG(LogTemp, Warning, TEXT("ActiveStatusEffects: %d"), ActiveStatusEffects.Num());*/

		//Initializing TimeRemaining to Start at Effect Duration
		Effect.TimeRemaining = Effect.Duration;
		
		/*Handle Any Initial Damage*/
		HandleEffectInitialDamage(Effect);
		
		HandleSlowTags(Effect, 0.5f); //50% slower
	}
	else
	{
		HandleIsStackableEffect(Effect);
	}
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

bool UStatusEffectHandlerComponent::CheckIfTagAlreadyExists(FGameplayTag TagToCheck)
{
	//It is a unique tag and should be exact.
	 return StatusEffectIdTagContainer.HasTagExact(TagToCheck);
}

void UStatusEffectHandlerComponent::HandleIsStackableEffect(FStatusEffect& Effect)
{
	if (Effect.TypeTagContainer.HasTag(FGameplayTag::RequestGameplayTag("BuildableEffects.Type.Stackable")))
	{
		//Look through the Active Status Effects TArray for the Effect with the Effect Id Tag for this effect.
		FStatusEffect* StackableEffect = FindEffectByIdTag(Effect.EffectIdTag);

		if (StackableEffect && StackableEffect->CurrentStacks < StackableEffect->MaxStacks)
		{
			//Increment the stacks if possible.
			StackableEffect->CurrentStacks++;
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
			UE_LOG(LogTemp, Warning, TEXT("Slowed Enemy: %s"), *OwningEnemyCharacter->GetName());
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
			//Handle Any DOT Tags
			//Logic...
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Removing Effect."));
			StagedForRemoval.Add(StatusEffect);
		}
	}

	//Handles Removal of all Staged Removal Effects
	RemoveMultipleEffectsFromComponent(StagedForRemoval);
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

FStatusEffect* UStatusEffectHandlerComponent::FindEffectByIdTag(const FGameplayTag& Tag)
{
	return ActiveStatusEffects.FindByPredicate([&Tag](const FStatusEffect& Effect)
	{
		return Effect.EffectIdTag == Tag;
	});
}

