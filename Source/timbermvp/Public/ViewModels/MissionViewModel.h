// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Data/DataAssets/MissionSystem/MissionBase.h"
#include "MissionViewModel.generated.h"

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UMissionViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:

	void GenerateMissionObjectiveString(UMissionBase* ActiveMission);
	
	FString GetMissionTitle() const;
	void SetMissionTitle(FString NewMissionTitle);
	
	FString GetMissionDescription() const;
	void SetMissionDescription(FString NewMissionDescription);
	
	float GetProgressPercent() const;
	void SetProgressPercent(float NewProgressPercent);
	
	int GetProgressAmount() const;
	void SetProgressAmount(int NewProgressAmount);

	EMissionState GetActiveMissionState() const;
	void SetActiveMissionState(EMissionState NewActiveMissionState);

	FString GetMissionObjectiveString() const;
	void SetMissionObjectiveString(FString NewMissionObjectiveString);

protected:

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter)
	EMissionState ActiveMissionState = EMissionState::Default;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter)
	FString MissionTitle = "Test Mission Title";

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter)
	FString MissionDescription = "Mission Description Goes Here.";
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter)
	float ProgressPercent = 0.f;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter)
	int ProgressAmount = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter)
	FString MissionObjectiveString = "Mission Objective";

	//TODO:: Have an Objective String. With Dynamic Values Highlighted.
	// EX. Destroy [10] [Grunts] with [Hammer] (0/10)
	// EX. Destroy {Count} {Enemy.Carbonite.Grunts} with {Weapon.Hammer} 
	// Content in [content] to be highlightable.
	// Multiple Objectives Should Stack.

	
	
};
