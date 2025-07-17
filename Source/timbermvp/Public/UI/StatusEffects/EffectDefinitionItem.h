// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/DataAssets/StatusEffects/StatusEffectBase.h"
#include "Data/DataAssets/StatusEffects/EffectConditions/EffectConditionBase.h"
#include "EffectDefinitionItem.generated.h"


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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Data", meta = (BindWidget))
	UTextBlock* EffectNameTextBlock;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect Data")
	FText EffectName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Data", meta = (BindWidget))
	UTextBlock* ConditionValueTextBlock;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect Data")
	FText ConditionValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Data", meta = (BindWidget))
	UTextBlock* InitialDamageTextBlock;

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
	
	TArray<FName> ModifierList;
	TArray<FName> RemovesEffectTagList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusEffects")
	FLinearColor EffectColor;
	
private:
	
	void CheckForInitialDamage(FStatusEffect& StatusEffect);
	void CheckForModifiers(FStatusEffect& StatusEffect);
	void HandleEffectConditionType(UEffectConditionBase* EffectCondition);
	void GetEffectName(FStatusEffect& StatusEffect);
	void ProcessDOTInfo(FStatusEffect& StatusEffect);
	
	void ConfigureMinorEffect();
	void ConfigureMajorEffect();
	void ConfigureUltimateEffect(FStatusEffect& StatusEffect);
	
};
