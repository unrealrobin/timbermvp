// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildSystem/BuildingComponents/TimberBuildingComponentBase.h"
#include "BuildingComponentPanel.generated.h"

class FAssetRegistryModule;

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UBuildingComponentPanel : public UUserWidget
{
	GENERATED_BODY()

public:

protected:

	UFUNCTION()
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void LoadAllDataAssetsForMenu();

	UFUNCTION(BlueprintImplementableEvent)
	void CreateBuildableIconsInMenu();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Buildables")
	TArray<UDataAsset*> AllBuildablesArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FAssetData> AssetDataArray;
	
};
