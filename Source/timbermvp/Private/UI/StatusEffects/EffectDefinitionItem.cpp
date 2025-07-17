// Property of Paracosm.


#include "UI/StatusEffects/EffectDefinitionItem.h"

#include "Components/TextBlock.h"
#include "Data/DataAssets/StatusEffects/StatusEffectDefinition.h"
#include "Data/DataAssets/StatusEffects/EffectConditions/EffectCondition_Chance.h"
#include "Data/DataAssets/StatusEffects/EffectConditions/EffectCondition_HasTag.h"
#include "Data/DataAssets/StatusEffects/EffectConditions/EffectCondition_HealthThreshold.h"
#include "Data/DataAssets/StatusEffects/EffectConditions/EffectCondition_StackCount.h"
#include "Subsystems/SynergySystem/SynergySystem.h"

void UEffectDefinitionItem::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEffectDefinitionItem::ConfigureDataDisplay()
{
	//This is the creation of a Single Effect Here. Can be Burn, Scorch, or Ignite. Must Accomodate for Each Scenario.
	SynSub = GetGameInstance()->GetSubsystem<USynergySystem>();

	if (EffectDefinition && SynSub)
	{
		FStatusEffect StatusEffectStruct = EffectDefinition->StatusEffectAsset->StatusEffect;
		EStatusEffectLevel EffectLevel = StatusEffectStruct.EffectLevel;
		EffectColor = StatusEffectStruct.GetEffectColor();

		GetEffectName(StatusEffectStruct);
		HandleEffectConditionType(EffectDefinition->StatusEffectCondition);
		CheckForInitialDamage(StatusEffectStruct);
		CheckForModifiers(StatusEffectStruct);
		HandleEffectConditionType(EffectDefinition->StatusEffectCondition);

		if (EffectLevel == EStatusEffectLevel::Ultimate)
		{
			ConfigureUltimateEffect(StatusEffectStruct);
		}
	}
	else
	{
		if (!SynSub)
		{
			UE_LOG(LogTemp, Warning, TEXT("EffectDefinition or SynergySystem is not valid."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("EffectDefinition is not valid."));
		}
	}
}

void UEffectDefinitionItem::CheckForInitialDamage(FStatusEffect& StatusEffect)
{
	if (StatusEffect.InitialDamage > 0)
	{
		InitialDamageTextBlock->SetText(FText::AsNumber(StatusEffect.InitialDamage));
	}
}

void UEffectDefinitionItem::CheckForModifiers(FStatusEffect& StatusEffect)
{
	for (FGameplayTag ModifierTag : StatusEffect.ModifierTagContainer)
	{
		if (SynSub->GetLastNameOfGameplayTag(ModifierTag) == FName("IsSlowed"))
		{
			ModifierList.Add(FName("Slow"));
			continue;
		}

		if (SynSub->GetLastNameOfGameplayTag(ModifierTag) == FName("IsStunned"))
		{
			ModifierList.Add(FName("Stun"));
			continue;
		}

		if (SynSub->GetLastNameOfGameplayTag(ModifierTag) == FName("HasDOT"))
		{
			ModifierList.Add(FName("DOT"));
			ProcessDOTInfo(StatusEffect);
			continue;
		}

		if (SynSub->GetLastNameOfGameplayTag(ModifierTag) == FName("IsTakesMoreDamage"))
		{
			ModifierList.Add(FName("Takes Increased Damage"));
			continue;
		}

		if (SynSub->GetLastNameOfGameplayTag(ModifierTag) == FName("IsIncreasedSpeed"))
		{
			ModifierList.Add(FName("Speed Increased"));
			continue;	
		}

		if (SynSub->GetLastNameOfGameplayTag(ModifierTag) == FName("Removal"))
		{
			FGameplayTag RemovedEffectTag = StatusEffect.TagsToRemove.First();
			FName RemovedEffectName = SynSub->GetLastNameOfGameplayTag(RemovedEffectTag);
			RemovesEffectTagList.Add(RemovedEffectName);
		}
	}
}

void UEffectDefinitionItem::HandleEffectConditionType(UEffectConditionBase* EffectCondition)
{
	EEffectConditionType EffectConditionType = EffectCondition->EffectConditionType;
	switch (EffectConditionType)
	{
	case EEffectConditionType::Always:
		ConditionValue = FText::FromString("Always Triggers");
		break;
	case EEffectConditionType::HasTag:
		if (UEffectCondition_HasTag* HasTagCondition = Cast<UEffectCondition_HasTag>(EffectCondition))
		{
			if (SynSub)
			{
				FName TagName = SynSub->GetLastNameOfGameplayTag(HasTagCondition->Tag);
				ConditionValue = FText::FromName(TagName);
			}
			else
			{
				ConditionValue = FText::FromString("Enemy Has Tag");
			}
		}
		break;
	case EEffectConditionType::StackCount:
		if (UEffectCondition_StackCount* StackCountCondition = Cast<UEffectCondition_StackCount>(EffectCondition))
		{
			ConditionValue = FText::AsNumber(StackCountCondition->StackCount);
		}
		break;
	case EEffectConditionType::HealthThreshold:
		if (UEffectCondition_HealthThreshold* HealthThresholdCondition = Cast<UEffectCondition_HealthThreshold>(EffectCondition))
		{
			ConditionValue = FText::AsNumber(HealthThresholdCondition->HealthThreshold);
		}
		break;
	case EEffectConditionType::Chance:
		if (UEffectCondition_Chance* ChanceCondition = Cast<UEffectCondition_Chance>(EffectCondition))
		{
			ConditionValue = FText::AsNumber(ChanceCondition->Chance);
		}
		break;
	case EEffectConditionType::Default:
		break;
	}
}

void UEffectDefinitionItem::GetEffectName(FStatusEffect& StatusEffect)
{
	if (SynSub)
	{
		FName Name = SynSub->GetLastNameOfGameplayTag(StatusEffect.EffectIdTag);
		EffectName = FText::FromName(Name);
	}
	else
	{
		EffectName = FText::FromString("EFFECT_NAME");
	}
}

void UEffectDefinitionItem::ProcessDOTInfo(FStatusEffect& StatusEffect)
{
	DamagePerTickValue = StatusEffect.DamagePerTick;
	Duration = StatusEffect.Duration;
	TickInterval = StatusEffect.TickInterval;
}

void UEffectDefinitionItem::ConfigureMinorEffect()
{
}

void UEffectDefinitionItem::ConfigureMajorEffect()
{
}

void UEffectDefinitionItem::ConfigureUltimateEffect(FStatusEffect& StatusEffect)
{
	//Create a Emergent Tag Widget
}

