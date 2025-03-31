// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "SaveLoadStruct.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveLoadSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TIMBERMVP_API USaveLoadSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	/*Save/Load Slots*/
	FString DEVELOPER_SAVE_BASE_MIDGAME = TEXT("DEVELOPER_BASE_SAVE");
	FString MidGameDemoSaveSlot = TEXT("MidGameDemoSaveSlot");
	FString StandardSaveSlot = TEXT("StandardSaveSlot");

	UFUNCTION()
	FString GetSaveSlot();

	/*Stores a Key Value Pair of Guid->Buildables*/
	UPROPERTY()
	TMap<FGuid, ABuildableBase*> GuidToBuildableMap;
	
	//Register Buildables - Add to TMap
	void RegisterBuildable();
	//DeRegister Buildables - Remove Buildables from TMap
	void DeRegisterBuildable();
	//Move all Save and Load functionality here.

	/* Save System*/
	UFUNCTION(BlueprintCallable, Category="Save System")
	void SaveCurrentGame();
	void CheckBuildingComponentForSnapAttachments(
		FBuildableData& BuildableData, ATimberBuildingComponentBase* BuildingComponent);
	void SaveBuildableData(USaveLoadStruct* SaveGameInstance);
	void SaveWaveData(USaveLoadStruct* SaveGameInstance);
	void SavePlayerData(USaveLoadStruct* SaveGameInstance);
	void SaveSeedaData(USaveLoadStruct* SaveGameInstance);

	/*Load System*/
	UFUNCTION(BlueprintCallable, Category="Save System")
	void LoadGame();
	void LoadBuildingComponents(USaveLoadStruct* LoadGameInstance);
	void LoadWaveData(USaveLoadStruct* LoadGameInstance);
	void LoadPlayerState(USaveLoadStruct* LoadGameInstance);
	void LoadSeedaData(USaveLoadStruct* LoadGameInstance);
	
};
