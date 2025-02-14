// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/BuildableBase.h"
#include "Engine/DataAsset.h"
#include "BuildComponentDataAsset.generated.h"

UENUM(BlueprintType) // Allows usage in Blueprints
enum class EBuildableClassification : uint8
{
	BuildingComponent UMETA(DisplayName = "Building Component"), // Enum value with display name
	Trap UMETA(DisplayName = "Trap"),
	Construct UMETA(DisplayName = "Construct"),
	Default UMETA(DisplayName = "Default")
};
/**
 * 
 */
UCLASS()
class TIMBERMVP_API UBuildComponentDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBuildableClassification BuildableClassification = EBuildableClassification::Default;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABuildableBase> BuildingComponentClass;
};
