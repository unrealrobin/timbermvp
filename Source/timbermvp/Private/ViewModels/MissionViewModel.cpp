// Property of Paracosm.


#include "ViewModels/MissionViewModel.h"

FString UMissionViewModel::GetMissionTitle() const
{
	return MissionTitle;
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
