// Property of Paracosm Industries.


#include "UI/BuildingComponentPanel.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "BuildSystem/Traps/TrapBase.h"
#include "Components/TextBlock.h"
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
	//Initially Collapsed - Made Visible when Utilized.
	InitialDamageTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	DOTTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	DOTDamageTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	DOTDurationTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	MaxStacksTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	SlowsTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	MetaTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	RemovesTextBlock->SetVisibility(ESlateVisibility::Collapsed);

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

					//Initial Damage Data
					if (StatusEffectData.InitialDamage > 0.f)
					{
						InitialDamageTextBlock->SetVisibility(ESlateVisibility::Visible);
						InitialDamage = StatusEffectData.InitialDamage;
						FText Combined = FText::Format(FText::FromString("Initial Damage: {0}"), FText::AsNumber(InitialDamage));
						InitialDamageTextBlock->SetText(Combined);
					}
					else
					{
						InitialDamageTextBlock->SetVisibility(ESlateVisibility::Collapsed);
					}

					/*if (StatusEffectData.TypeTagContainer.Num() > 0)
					{
						for (FGameplayTag Tag : StatusEffectData.TypeTagContainer)
						{
							if (Tag == FGameplayTag::RequestGameplayTag("BuildableEffects.Type.DOT"))
							{
								DOTDamageTextBlock->SetVisibility(ESlateVisibility::Visible);
								DamagePerTick = StatusEffectData.DamagePerTick;
								FText Combined = FText::Format(FText::FromString("Damage Per Sec: {0}"), FText::AsNumber(DamagePerTick));
								DOTDamageTextBlock->SetText(Combined);
								
								DOTDurationTextBlock->SetVisibility(ESlateVisibility::Visible);
								EffectDuration = StatusEffectData.Duration;
								FText CombinedDuration = FText::Format(FText::FromString("Duration: {0}"), FText::AsNumber(EffectDuration));
								DOTDurationTextBlock->SetText(CombinedDuration);
							}
							else if (Tag == FGameplayTag::RequestGameplayTag("BuildableEffects.Type.Stackable"))
							{
								MaxStacks = StatusEffectData.MaxStacks;
								MaxStacksTextBlock->SetVisibility(ESlateVisibility::Visible);
								FText Combined = FText::Format(FText::FromString("Max Stacks: {0}"), FText::AsNumber(MaxStacks));
								MaxStacksTextBlock->SetText(Combined);
							}
							else if (Tag == FGameplayTag::RequestGameplayTag("BuildableEffects.Type.Slow"))
							{
								bSlows = true;
								SlowsTextBlock->SetVisibility(ESlateVisibility::Visible);
								SlowsTextBlock->SetText(FText::FromString("Applies: Slow"));
							}
							
						}
					}*/

					//Array for storing all Removal Effect Types
					//TODO:: This wont appear nicely if Removal has multiple types/effects.
					// We can add a loop to iterate through the MetaTagContainer if needed with proper formatting conditions to handle additional removal types.
					/*if (StatusEffectData.MetaTagContainer.Num() > 0)
					{
						RemovesTextBlock->SetVisibility(ESlateVisibility::Visible);
						RemovesTextBlock->SetText(FText::FromString("Removes: Slow"));
					}*/
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
