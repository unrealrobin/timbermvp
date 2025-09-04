// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "MissionBase.generated.h"

UENUM(BlueprintType) 
enum class EMissionState : uint8
{
	Incomplete UMETA(DisplayName = "Not Complete"),
	InProgress UMETA(DisplayName = "In Progress"),
	Complete UMETA(DisplayName = "Complete"),
	Default UMETA(DisplayName = "Default")
};

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UMissionBase : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mission Info")
	FGuid MissionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission Info")
	FText MissionTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission Info")
	FText MissionDescription;

	//Ex. Kills, Builds, Timer, other..
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission Info")
	FGameplayTag MissionEventType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mission Info")
	EMissionState MissionState = EMissionState::Incomplete;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission Info")
	bool MultipleObjectives = false;
	
};
