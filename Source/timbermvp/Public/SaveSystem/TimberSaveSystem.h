// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/TimberBuildingComponentBase.h"
#include "GameFramework/SaveGame.h"
#include "TimberSaveSystem.generated.h"

USTRUCT(BlueprintType)
struct FBuildingComponentData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TSubclassOf<ATimberBuildingComponentBase> BuildingComponentClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FTransform BuildingComponentTransform;
};

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
	float WaveNumber = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Building Component Save Data")
	TArray<FBuildingComponentData> BuildingComponentsArray;
};
