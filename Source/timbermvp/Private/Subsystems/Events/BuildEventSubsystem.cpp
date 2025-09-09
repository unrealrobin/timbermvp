// Property of Paracosm.


#include "Subsystems/Events/BuildEventSubsystem.h"

void UBuildEventSubsystem::BroadcastBuildEvent(FMissionEventPayload Payload)
{
	OnBuildEvent.Broadcast(Payload);

	UE_LOG(LogTemp, Warning, TEXT("Build Event Broadcasted."));
}
