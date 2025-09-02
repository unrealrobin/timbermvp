// Property of Paracosm.


#include "Subsystems/Events/BuildEventSubsystem.h"

void UBuildEventSubsystem::HandleBuildEvent(FMissionEventPayload Payload)
{
	OnBuildEvent.Broadcast(Payload);

	UE_LOG(LogTemp, Warning, TEXT("Build Event Broadcasted."));
}
