// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/DataAssets/StatusEffects/StatusEffectBase.h"
#include "Data/DataAssets/StatusEffects/EffectConditions/EffectConditionBase.h"
#include "EffectDefinitionItem.generated.h"


class UHorizontalBox;
class UWrapBox;
enum class EEffectConditionType : uint8;
class USynergySystem;
class ATrapBase;
class UStatusEffectDefinition;
class UTextBlock;

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UEffectDefinitionItem : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void ConfigureDataDisplay();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StatusEffects")
	UStatusEffectDefinition* EffectDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StatusEffects")
	ATrapBase* TrapDefaultObject = nullptr;

	UPROPERTY()
	USynergySystem* SynSub = nullptr;

protected:
	/*Effect Level Data*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Data", meta = (BindWidget))
	UTextBlock* EffectLevelTextBlock;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect Data")
	FText EffectLevelValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StatusEffects")
	EStatusEffectLevel EffectLevelType;
	/* Names */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Data", meta = (BindWidget))
	UTextBlock* EffectNameTextBlock;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect Data")
	FText EffectName;
	/*Condition for Effect*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Data", meta = (BindWidget))
	UTextBlock* ConditionValueTextBlock;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect Data")
	FText ConditionValue;
	/*InitialDamage*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Data", meta = (BindWidget))
	UTextBlock* InitialDamageTextBlock;
	/*DOT Info*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Data", meta = (BindWidget))
	UTextBlock* DamagePerTickTextBlock;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect Data")
	float DamagePerTickValue = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Data", meta = (BindWidget))
	UTextBlock* DOTDurationTextBlock;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect Data")
	float Duration = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Data", meta = (BindWidget))
	UTextBlock* TickIntervalTextBlock;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect Data")
	float TickInterval = 0.f;
	/*Effect Modifiers*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Data", meta = (BindWidget))
	UHorizontalBox* ModifierListContainer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Data", meta = (BindWidget))
	TArray<FName> ModifierList;
	/*Modifier Removal Tags*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Data", meta = (BindWidget))
	UHorizontalBox* RemovalEffectTagList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Data", meta = (BindWidget))
	TArray<FName> RemovesEffectTagList;
	/*Colors*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusEffects")
	FLinearColor EffectColor;

	//Holds containers with FGameplayTags that when combined create some Emergent Tag.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StatusEffects")
	TArray<FGameplayTagContainer> EmergentTagRuleContainers;

	//TODO:: Fix Later as I am Hard Coding Emergent Rule Value linking here.
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EmergentTagRuleNames")
	FText ConditionEmergentTag;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EmergentTagRuleNames")
	FText Rule1Value1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EmergentTagRuleNames")
	FText Rule1Value2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EmergentTagRuleNames")
	FText Rule2Value1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EmergentTagRuleNames")
	FText Rule2Value2;
	
private:
	void SetEffectLevelInfo(EStatusEffectLevel EffectLevel);
	void CheckForInitialDamage(FStatusEffect& StatusEffect);
	void CheckForModifiers(FStatusEffect& StatusEffect);
	void HandleEffectConditionType(UEffectConditionBase* EffectCondition);
	void GetEffectName(FStatusEffect& StatusEffect);
	void ProcessDOTInfo(FStatusEffect& StatusEffect);
	void GetTagsForEmergentCombos(UStatusEffectDefinition* Def);
	
};
