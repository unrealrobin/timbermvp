// Property of Paracosm Industries.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/BuildingComponents/TimberBuildingComponentBase.h"
#include "Components/Inventory/InventoryObject.h"
#include "GameFramework/SaveGame.h"
#include "TimberSaveSystem.generated.h"

USTRUCT(BlueprintType)
struct FInventoryData
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int NumberOfParts;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int NumberOfMechanism;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int NumberOfUniques;

	FInventoryData() :
		NumberOfParts(0), NumberOfMechanism(0), NumberOfUniques(0)
	{}
};

USTRUCT(BlueprintType)
struct FBuildingComponentData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TSubclassOf<ABuildableBase> BuildingComponentClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FTransform BuildingComponentTransform;

	//Default Constructor
	FBuildingComponentData() :
		BuildingComponentClass(nullptr), BuildingComponentTransform(FTransform::Identity)
	{}
};

USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector PlayerLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FInventoryData PlayerInventory;

	FPlayerData() :
		PlayerLocation(FVector::ZeroVector), PlayerInventory(FInventoryData())
	{}
	
};

USTRUCT(BlueprintType)
struct FSeedaData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector SeedaLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FRotator SeedaRotation;

	FSeedaData() :
		SeedaLocation(FVector::ZeroVector), SeedaRotation(FRotator::ZeroRotator)
	{}
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Seeda Save Data")
	FSeedaData SeedaData;
};
