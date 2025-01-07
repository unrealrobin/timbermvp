// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/BuildingComponents/TimberBuildingComponentBase.h"
#include "Engine/DataAsset.h"
#include "BuildComponentDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UBuildComponentDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATimberBuildingComponentBase> BuildingComponentClass;
};
