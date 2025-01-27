// Property of Paracosm Industries.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/BuildingComponents/TimberBuildingComponentBase.h"
#include "GameFramework/SaveGame.h"
#include "TimberSaveSystem.generated.h"

USTRUCT(BlueprintType)
struct FBuildingComponentData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TSubclassOf<ABuildableBase> BuildingComponentClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FTransform BuildingComponentTransform;
};

USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FTransform PlayerTransform;
	
};

/*
 *
 * Things that should be Saved.
 * 1. Player Location.
 * 2. All Building Components.
 * 2a. Building Component Durability?
 * 3. All of Players Inventory.
 * 4. Wave Start State
 * 4a. Wave Number, Door Positions, All Array contents Empty.
 * 
 */

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UTimberSaveSystem : public USaveGame
{
	GENERATED_BODY()

public:
	//Save Game Variables Here.

	UPROPERTY(VisibleAnywhere, Category="Wave Data")
	int WaveNumber = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Building Component Save Data")
	TArray<FBuildingComponentData> BuildingComponentsArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Player Save Data")
	FPlayerData PlayerData;
};
