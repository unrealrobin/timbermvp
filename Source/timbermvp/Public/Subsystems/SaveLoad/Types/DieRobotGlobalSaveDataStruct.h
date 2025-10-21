#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DieRobotGlobalSaveDataStruct.generated.h"
// Used to Save Global Data for Player.*/

USTRUCT(BlueprintType, meta=(scriptName="GlobalSaveDataStruct"))
struct FDieRobotGlobalSaveDataStruct
{
	GENERATED_BODY()

	UPROPERTY()
	FString LastSavedGame;
};

UCLASS()
class TIMBERMVP_API UDieRobotGlobalSaveData : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FDieRobotGlobalSaveDataStruct Data;
};