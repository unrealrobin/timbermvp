// Property of Paracosm.


#include "ViewModels/MissionViewModel.h"

void UMissionViewModel::GenerateMissionObjectiveString(UMissionBase* ActiveMission)
{
	if (IsValid(ActiveMission))
	{
		FText ObjectiveText;

		if (ActiveMission->MissionEventType == FGameplayTag::RequestGameplayTag("Event.Combat.Destroy"))
		{
			// Now we need to take into consideration whether the other Context Tags Exist. Which we don't like. 
		}
	}
}

FString UMissionViewModel::GetMissionTitle() const
{
	return MissionTitle;
}

void UMissionViewModel::SetMissionTitle(FString NewMissionTitle)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MissionTitle, NewMissionTitle))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(MissionTitle);
	}
	//MissionTitle = NewMissionTitle;
}
void UMissionViewModel::SetMissionDescription(FString NewMissionDescription)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MissionDescription, NewMissionDescription))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(MissionDescription);
	}
	//MissionDescription = NewMissionDescription;
}
void UMissionViewModel::SetProgressAmount(int NewProgressAmount)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(ProgressAmount, NewProgressAmount))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(ProgressAmount);
	}
	//ProgressAmount = NewProgressAmount;
}

EMissionState UMissionViewModel::GetActiveMissionState() const
{
	return ActiveMissionState;
}

void UMissionViewModel::SetActiveMissionState(EMissionState NewActiveMissionState)
{
	ActiveMissionState = NewActiveMissionState;
}

FString UMissionViewModel::GetMissionObjectiveString() const
{
	return MissionObjectiveString;
}

void UMissionViewModel::SetMissionObjectiveString(FString NewMissionObjectiveString)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MissionObjectiveString, NewMissionObjectiveString))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(MissionObjectiveString);
	}
}

void UMissionViewModel::SetProgressPercent(float NewProgressPercent)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(ProgressPercent, NewProgressPercent))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(ProgressPercent);
	}
	//ProgressPercent = NewProgressPercent;
}

FString UMissionViewModel::GetMissionDescription() const
{
	return MissionDescription;
}

float UMissionViewModel::GetProgressPercent() const
{
	return ProgressPercent;
}

int UMissionViewModel::GetProgressAmount() const
{
	return ProgressAmount;
}
