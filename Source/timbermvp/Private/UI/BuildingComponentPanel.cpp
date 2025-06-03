// Property of Paracosm Industries. Dont use my shit.


#include "UI/BuildingComponentPanel.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Data/DataAssets/BuildComponentDataAsset.h"
#include "Data/DataAssets/StatusEffects/StatusEffectBase.h"


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

void UBuildingComponentPanel::PrepareStatusEffectForMenu()
{
	if (BuildMenuHoveredIconDataAsset){
		if (UBuildComponentDataAsset* BuildComponentDataAsset = Cast<UBuildComponentDataAsset>(BuildMenuHoveredIconDataAsset))
		{
			if (BuildComponentDataAsset->BuildingComponentClass->IsChildOf(ATrapBase::StaticClass()))
			{
				ATrapBase* TrapBase = BuildComponentDataAsset->BuildingComponentClass->GetDefaultObject<ATrapBase>();
				if (TrapBase && TrapBase->StatusEffectDataAsset)
				{
					bHasStatusEffect = true;
					//here we have access to the status effect of the trap
					FStatusEffect StatusEffectData = TrapBase->StatusEffectDataAsset->StatusEffect;

					if (StatusEffectData.InitialDamage > 0.f)
					{
						InitialDamage = StatusEffectData.InitialDamage;
					}

					for (FGameplayTag Tag : StatusEffectData.TypeTagContainer)
					{
						if (Tag == FGameplayTag::RequestGameplayTag("BuildableEffects.Type.DOT"))
						{
							DamagePerTick = StatusEffectData.DamagePerTick;
							EffectDuration = StatusEffectData.Duration;
						}
						else if (Tag == FGameplayTag::RequestGameplayTag("BuildableEffects.Type.Stackable"))
						{
							MaxStacks = StatusEffectData.MaxStacks;
						}
						else if (Tag == FGameplayTag::RequestGameplayTag("BuildableEffects.Type.Slow"))
						{
							bSlows = true;
						}
					}

					for (FGameplayTag Tag : StatusEffectData.MetaTagContainer)
					{
						if (Tag == FGameplayTag::RequestGameplayTag("BuildableEffects.Meta.Removes.Slow"))
						{
							bRemovesSlow = true;
						}
						
					}
				}
			}
		
		}
	}
	else
	{
		bHasStatusEffect = false;
		UE_LOG(LogTemp, Warning, TEXT("Hovered Building Component Icon Class has NO Status Effect Data Asset!"));
	}
}

void UBuildingComponentPanel::SetHasStatusEffect(bool bNewHasStatusEffect)
{
	bHasStatusEffect = bNewHasStatusEffect;
}

void UBuildingComponentPanel::NativeConstruct()
{
	Super::NativeConstruct();
	//Getting all the Data Assets for the Building Components
	LoadAllDataAssetsForMenu();
	//Creating the Icons in the Menu
	CreateBuildableIconsInMenu();
}
