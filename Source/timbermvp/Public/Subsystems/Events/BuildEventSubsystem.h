// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Types/MissionEventPayloads/MissionEventPayload.h"
#include "BuildEventSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UBuildEventSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildEvent, FMissionEventPayload, Payload);
	FOnBuildEvent OnBuildEvent;

	void HandleBuildEvent(FMissionEventPayload Payload);
};
