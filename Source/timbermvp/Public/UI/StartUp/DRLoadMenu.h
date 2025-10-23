// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DRLoadMenu.generated.h"

class UVerticalBox;
class UDRLoadItem;
struct FSaveSlotDataStruct;
/**
 * 
 */
UCLASS()
class TIMBERMVP_API UDRLoadMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void GetAllSavedGames();
	virtual void NativeConstruct() override;
	
	void DisplayAllSavedGames();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget Data")
	TSubclassOf<UDRLoadItem> LoadItemWidgetClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalLoadItemBlock;
	
private:
	UPROPERTY()
	TArray<FSaveSlotDataStruct> AllSavedGames;

};
