// Property of Paracosm Industries. Dont use my shit.


#include "UI/BuildingComponentPanel.h"
#include "AssetRegistry/AssetRegistryModule.h"



void UBuildingComponentPanel::LoadAllDataAssetsForMenu()
{
	//TO BE CALLED FROM THE GAME MODE BEGIN PLAY
	FString PathToBuildableDataAssets = TEXT("/Game/BuildingComponents/DataAssets/BuildingComponentDataAsset");
	//Loading Asset Registry Module
	FAssetRegistryModule& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	if (AssetRegistry.IsValid())
	{
		//Scan the folder with the Data Assets / ensures the assets are registered.
		//Scan is slow when folder is large - to be considered in the future with many buildables.
		AssetRegistry.Get().ScanPathsSynchronous({PathToBuildableDataAssets});

		//Get all assets at this path, and populate the AssetData (not data asset) into AssetDataArray<FAssetData>
		AssetRegistry.Get().GetAssetsByPath(FName(*PathToBuildableDataAssets), AssetDataArray, false);

		for (FAssetData& AssetData : AssetDataArray)
		{
			UObject* LoadedAsset = AssetData.GetAsset();
			if (LoadedAsset)
			{
				UDataAsset* DataAsset = Cast<UDataAsset>(LoadedAsset);
				if (DataAsset)
				{
					AllBuildablesArray.Add(DataAsset);
				}
			}
		}
	}
}

void UBuildingComponentPanel::NativeConstruct()
{
	Super::NativeConstruct();
	//Getting all the Data Assets for the Building Components
	LoadAllDataAssetsForMenu();
	//Creating the Icons in the Menu
	CreateBuildableIconsInMenu();
}
