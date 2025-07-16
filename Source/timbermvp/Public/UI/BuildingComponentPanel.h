// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingComponentPanel.generated.h"

class FAssetRegistryModule;
class ATrapBase;
class UTextBlock;

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UBuildingComponentPanel : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	void ToggleStatusEffectDetails();

protected:

	UFUNCTION()
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void LoadAllDataAssetsForMenu();

	UFUNCTION(BlueprintImplementableEvent)
	void CreateBuildableIconsInMenu();

	UFUNCTION(BlueprintCallable)
	void PrepareStatusEffectForMenu();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Build Menu")
	UDataAsset* BuildMenuHoveredIconDataAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Build Menu")
	bool bIsIconFocusedOrHovered = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Buildables")
	TArray<UDataAsset*> AllBuildablesArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FAssetData> AssetDataArray;

	/*Status Effect Vars*/
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status Effects")
	bool bHasStatusEffect = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status Effects")
	float InitialDamage = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status Effects")
	float DamagePerTick = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status Effects")
	float EffectDuration = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status Effects")
	int MaxStacks = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status Effects")
	bool bSlows = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status Effects")
	bool bRemovesSlow = false;

	UFUNCTION(BlueprintCallable)
	void SetHasStatusEffect(bool bNewHasStatusEffect);

	/* Build Menu Trap Data Panel */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InitialDamageTextBlock;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DOTTextBlock;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DOTDamageTextBlock;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DOTDurationTextBlock;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxStacksTextBlock;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SlowsTextBlock;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MetaTextBlock;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RemovesTextBlock;
	
};
