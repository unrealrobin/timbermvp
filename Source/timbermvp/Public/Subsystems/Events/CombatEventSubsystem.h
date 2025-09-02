// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Types/MissionEventPayloads/MissionEventPayload.h"
#include "CombatEventSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UCombatEventSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatEvent, FMissionEventPayload, Payload);
	FOnCombatEvent OnCombatEvent;

	void HandleCombatEvent(FMissionEventPayload Payload);
};
