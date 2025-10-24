// Property of Paracosm.


#include "UI/StartUp/DRLoadMenu.h"

#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Subsystems/SaveLoad/SaveLoadSubsystem.h"
#include "Subsystems/SaveLoad/Types/DieRobotGlobalSaveDataStruct.h"
#include "UI/StartUp/DRLoadItem.h"
#include "UI/StartUp/StartMenu.h"


void UDRLoadMenu::NativeConstruct()
{
	Super::NativeConstruct();

	GetAllSavedGames();
}

void UDRLoadMenu::HandleMainMenuButtonClicked()
{
	OnMainMenuButtonClickedDelegate.Broadcast();
}

void UDRLoadMenu::GetAllSavedGames()
{
	AllSavedGames.Empty();
	
	USaveLoadSubsystem* SLS = GetGameInstance()->GetSubsystem<USaveLoadSubsystem>();
	if (SLS)
	{
		AllSavedGames = SLS->GetGlobalSaveDataInstance()->ActiveSaveSlots;

		UE_LOG(LogTemp, Warning, TEXT("Current there are %d saved games locally."), AllSavedGames.Num());
	}
}


void UDRLoadMenu::DisplayAllSavedGames()
{
	GetAllSavedGames();
	
	if (AllSavedGames.Num() > 0)
	{
		for (FSaveSlotDataStruct SaveSlot: AllSavedGames)
		{
			UE_LOG(LogTemp, Warning, TEXT("Save Slot"));
			//Create Load Menu Item Widgets
			UUserWidget* Widget = CreateWidget(this, LoadItemWidgetClass);
			UDRLoadItem* LoadItemWidget = Cast<UDRLoadItem>(Widget);
			if (VerticalLoadItemBlock && LoadItemWidget)
			{
				LoadItemWidget->WaveTextBlock->SetText(FText::FromString(FString::FromInt(SaveSlot.SlotCurrentWave)));
				LoadItemWidget->TimeStampBlock->SetText(FText::FromString(SaveSlot.LastTimeStamp));
				LoadItemWidget->SaveSlotIdBlock->SetText(FText::FromString(SaveSlot.SlotName));
				
				VerticalLoadItemBlock->AddChild(LoadItemWidget);

				UE_LOG(LogTemp, Warning, TEXT("Added LoadItemWidget to Vertical Box."));
			}
		}
	}
}
