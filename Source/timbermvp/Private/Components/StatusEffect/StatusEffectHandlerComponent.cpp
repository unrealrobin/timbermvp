// Property of Paracosm.


#include "Components/StatusEffect/StatusEffectHandlerComponent.h"
#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Components/WidgetComponent.h"
#include "Data/DataAssets/StatusEffects/StatusEffectBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Subsystems/SynergySystem/SynergySystem.h"
#include "Types/Combat/DamagePayload.h"
#include "UI/StatusEffects/StatusEffectBar.h"

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
	for (FGameplayTag Tag : StatusEffect.ModifierTagContainer)
	{
		if (Tag == FGameplayTag::RequestGameplayTag("SynergySystem.Modifier.Debuff.HasDOT"))
		{
			StatusEffect.TickAccumulator += DeltaTime;
			if (StatusEffect.TickAccumulator >= StatusEffect.TickInterval)
			{
				//Stacks multiply the Damage per Tick.
				float TotalDamage = StatusEffect.DamagePerTick * StatusEffect.CurrentStacks;

				FDamagePayload Payload;
				Payload.DamageAmount = TotalDamage;
				Payload.StatusEffect = StatusEffect;
				
				OwningEnemyCharacter->TakeDamage(Payload);
				
				//UE_LOG(LogTemp, Warning, TEXT ("Applied Total Damage Dot: %f"), TotalDamage);
				//Resetting the Tick Accumulator
				StatusEffect.TickAccumulator = 0;
			}
		}
	}
}

void UStatusEffectHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Going through Each Active Status Effect.
	for (FStatusEffect& StatusEffect : ActiveStatusEffects)
	{
		/*If the effect still has time remaining*/
		if (StatusEffect.TimeRemaining > 0.f)
		{
			//Incrementing
			StatusEffect.TimeRemaining -= DeltaTime;

			//Applying any DOT Effects if Valid
			HandleDotEffects(StatusEffect, DeltaTime);

			//HandleMetaPerpetualRemovals(StatusEffect);
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

void UStatusEffectHandlerComponent::HandleEffectInitialDamage(FStatusEffect& Effect)
{
	if (Effect.InitialDamage > 0.f)
	{
		FDamagePayload Payload;
		//Superfluous
		Payload.DamageAmount = Effect.InitialDamage;
		Payload.StatusEffect = Effect;
		OwningEnemyCharacter->TakeDamage(Payload);
	}
}

void UStatusEffectHandlerComponent::HandleMetaPerpetualRemovals(FStatusEffect& StatusEffect)
{
	//Looping through Type Tags Array of any Active Status Effects to Check if there is a Meta Tag
	for (FGameplayTag Tag : StatusEffect.MetaTagContainer) //Ex. Status Effect = Burn Effect 
	{
		//Does the Burn status effect have a Meta Tag that removes Slow?
		if (Tag == FGameplayTag::RequestGameplayTag("BuildableEffects.Meta.Perpetual.Removes.Slow")) // Burn effect has this Tag in the Meta Tag Container
		{
			
			//Check all existing Status Effects to see if there is a Slow Effect that needs to be removed.
			for (FStatusEffect& CheckedStatusEffect : ActiveStatusEffects)
			{
				if (CheckedStatusEffect.TypeTagContainer.HasTag(FGameplayTag::RequestGameplayTag("BuildableEffects.Type.Slow"))) 
				{
					StagedForRemoval.Add(CheckedStatusEffect);
					UE_LOG(LogTemp, Warning, TEXT("Removing : %s from: %s"), *CheckedStatusEffect.EffectIdTag.ToString(), *GetOwner()->GetName());
				}
			}
		}
	}
}

void UStatusEffectHandlerComponent::HandleMetaInitialRemovals(FStatusEffect& StatusEffect)
{
	//Looping through Type Tags Array of any Active Status Effects to Check if there is a Meta Tag
	for (FGameplayTag Tag : StatusEffect.ModifierTagContainer) //Ex. Status Effect = Burn Effect 
	{
		//Does the Burn status effect have a Meta Tag that removes Slow?
		if (Tag == FGameplayTag::RequestGameplayTag("SynergySystem.Modifier.Removal")) 
		{
			
			//Check all existing Status Effects to see if there is a Slow Effect that needs to be removed.
			for (FStatusEffect& CheckedStatusEffect : ActiveStatusEffects)
			{
				if (CheckedStatusEffect.TypeTagContainer.HasTag(FGameplayTag::RequestGameplayTag("BuildableEffects.Type.Slow"))) 
				{
					StagedForRemoval.Add(CheckedStatusEffect);
					UE_LOG(LogTemp, Warning, TEXT("Removing : %s from: %s"), *CheckedStatusEffect.EffectIdTag.ToString(), *GetOwner()->GetName());
				}
			}
		}
	}
}

void UStatusEffectHandlerComponent::HandleIsStackableEffect(FStatusEffect* Effect)
{
	if (Effect->TypeTagContainer.HasTag(FGameplayTag::RequestGameplayTag("BuildableEffects.Type.Stackable")))
	{
		//Look through the Active Status Effects TArray for the Effect with the Effect ID Tag for this effect.
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
	/*
	 * When adding a Slow tag, this modifies the Speed to .5x
	 * When removing it reverts back to 1.0x
	 *
	 * Potential issue here if more than 1 effect can apply a slow, or if there are stacks, this totally removes any slow effects.
	 * Potential Solution is we have Slow States, and they evaluate every tick. Expensive.
	 */
	for (FGameplayTag Tag : Effect.ModifierTagContainer)
	{
		if (Tag == FGameplayTag::RequestGameplayTag("SynergySystem.Modifier.Debuff.IsSlowed"))
		{
			OwningEnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = OwningEnemyCharacter->MaxWalkSpeedBase *  MaxWalkSpeedBaseMultiplier; 
			//UE_LOG(LogTemp, Warning, TEXT("Slowed Enemy: %s"), *OwningEnemyCharacter->GetName());
		}
	}
}

void UStatusEffectHandlerComponent::AddEffectToStatusEffectBar(FGameplayTag EffectTag)
{
	//TODO::This only works with the Enemy Characters At the moment. Interface? 
	ATimberEnemyCharacter* EnemyCharacter = Cast<ATimberEnemyCharacter>(GetOwner());
	if (EnemyCharacter)
	{
		if (EnemyCharacter->StatusEffectBarComponent)
		{
			UUserWidget* Widget = EnemyCharacter->StatusEffectBarComponent->GetUserWidgetObject();
			if (Widget)
			{
				UStatusEffectBar* StatusEffectBar = Cast<UStatusEffectBar>(Widget);
				if (StatusEffectBar)
				{
					StatusEffectBar->AddEffectToBar(EffectTag);
				}
			}
		}
	}
}

void UStatusEffectHandlerComponent::RemoveEffectFromStatusEffectBar(FGameplayTag EffectTag)
{
	ATimberEnemyCharacter* EnemyCharacter = Cast<ATimberEnemyCharacter>(GetOwner());
	if (EnemyCharacter)
	{
		if (EnemyCharacter->StatusEffectBarComponent)
		{
			UUserWidget* Widget = EnemyCharacter->StatusEffectBarComponent->GetUserWidgetObject();
			if (Widget)
			{
				UStatusEffectBar* StatusEffectBar = Cast<UStatusEffectBar>(Widget);
				if (StatusEffectBar)
				{
					StatusEffectBar->RemoveEffectFromBar(EffectTag);
				}
			}
		}
	}
}

void UStatusEffectHandlerComponent::AddStatusEffectToComponent(FStatusEffect& Effect, AActor* EffectInstigator)
{
	//Does this unique id already exist in the container?
	bool bTagExists = CheckIfTagAlreadyExists(Effect.EffectIdTag);

	if (!bTagExists) //Doesn't already exist
	{
		StatusEffectIdTagContainer.AddTag(Effect.EffectIdTag);
		
		ActiveStatusEffects.Add(Effect);

		//Initializing TimeRemaining to Start at Effect Duration - We subtract from Time Remaining Every Tick.
		Effect.TimeRemaining = Effect.Duration;
		
		/*Handle Any Initial Damage*/
		HandleEffectInitialDamage(Effect);

		/*Handle Any Initial Removals*/
		//HandleMetaInitialRemovals(Effect);

		//Handle Any Slow Tags on the Status Effect. (There can be multiple Tags on this Effect.)
		HandleSlowTags(Effect, 0.5f); //50% slower

		//This adds the Effect Icon to the Status Effect Bar on the Enemy Character.
		AddEffectToStatusEffectBar(Effect.EffectIdTag);
	}
	else
	{
		//If the ID Already existing in our array, we need to get the existing version of the effect. And add a stack. Then we need to reset the duration
		//because it's basically a new iteration of the effect at a higher stack.
		
		FStatusEffect* ExisitingStatusEffect = FindEffectByIdTag(Effect.EffectIdTag);
		if (ExisitingStatusEffect)
		{
			HandleIsStackableEffect(ExisitingStatusEffect);
		}
	}

	//TODO:: Pass Newly Added Tag and StatusEffectIdTagContainer to Synergy Subsystem to check for Lvl3 Synergies.
	ProcessTagForSynergy(Effect.EffectIdTag);
}

bool UStatusEffectHandlerComponent::CheckIfTagAlreadyExists(FGameplayTag TagToCheck)
{
	 return StatusEffectIdTagContainer.HasTagExact(TagToCheck);
}

void UStatusEffectHandlerComponent::RemoveEffectFromComponent(const FStatusEffect& Effect)
{
	/*
	 * Why do we Stage For Removal?
	 * We don't want to modify the ActiveStatusEffects array while iterating through it every tick. So we loop every status effect,
	 * and the last part of logic in tick clears any staged status effects for destruction.
	 */
	
	//Check the array for this effect and Remove it.

	//Remove Any Movement Speed Encumbrances applied during effect.
	HandleSlowTags(Effect, 1.0f); //Back to normal speed

	//Returns how Many Effects were removed.
	//This would remove all status effects, even if there are multiple stacks/instances of the same effect.(Imagine 2 Slow traps in the same location.)
	ActiveStatusEffects.RemoveAll([Effect](const FStatusEffect& ActiveEffect)
	{
		return ActiveEffect.EffectIdTag == Effect.EffectIdTag;
	});

	//Status Effect Bar UI Icon (Above Enemy Heads)
	RemoveEffectFromStatusEffectBar(Effect.EffectIdTag);
	
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

void UStatusEffectHandlerComponent::AddEmergentTag(FGameplayTag Tag, float Duration)
{
	StatusEffectIdTagContainer.AddTag(Tag);
	UE_LOG(LogTemp, Warning, TEXT("Status Effect Handler Component - Added Emergent Tag: %s"), *Tag.ToString());
	
	//TODO:: Have a visual representation of Emergent Tag.
	// Similar to floating damage numbers.

	if (OwningEnemyCharacter)
	{
		//TODO:: Update this to Match the Emergent Tags Color Identity.
		FName TagName = GetLastNameOfTag(Tag);
		OwningEnemyCharacter->SpawnEffectNameUI(TagName);
	}

	FTimerHandle EmergentTimerHandle;

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(EmergentTimerHandle, [this, Tag]()
		{
			if (StatusEffectIdTagContainer.HasTagExact(Tag))
			{
				StatusEffectIdTagContainer.RemoveTag(Tag);
			}
		}, Duration, false);
	}
}

FStatusEffect* UStatusEffectHandlerComponent::FindEffectByIdTag(const FGameplayTag& Tag)
{
	return ActiveStatusEffects.FindByPredicate([&Tag]( FStatusEffect& Effect)
	{
		return Effect.EffectIdTag == Tag;
	});
}

void UStatusEffectHandlerComponent::ProcessTagForSynergy(const FGameplayTag& Tag)
{
	//Call to Synergy Subsystem to check for Synergies.
	//Synergy subsystem handles the application of Tags - Effect Logic for Lvl 3 Synergies.
	USynergySystem* SynSub = GetWorld()->GetGameInstance()->GetSubsystem<USynergySystem>();
	if (SynSub)
	{
		SynSub->ProcessTagForSynergy(Tag, this);
	}
}

FName UStatusEffectHandlerComponent::GetLastNameOfTag(FGameplayTag Tag)
{
	FString TagName = Tag.GetTagName().ToString();
	TArray<FString> Parts;
	TagName.ParseIntoArray(Parts, TEXT("."), true);

	FName EffectName = Parts.Num() > 0 ? FName(*Parts.Last()) : NAME_None;

	return EffectName;
}





