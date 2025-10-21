#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GlobalSaveDataStruct.generated.h"
/* Used to Save Global Data for Player.*/

USTRUCT(BlueprintType)
struct FGlobalSaveDataStruct
{
	GENERATED_BODY()

	UPROPERTY()
	FString LastSavedGame;
};

UCLASS()
class TIMBERMVP_API UGlobalSaveData : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FGlobalSaveDataStruct Data;
};