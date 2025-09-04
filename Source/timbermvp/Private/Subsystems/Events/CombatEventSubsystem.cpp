// Property of Paracosm.


#include "Subsystems/Events/CombatEventSubsystem.h"

void UCombatEventSubsystem::BroadcastCombatEvent(FMissionEventPayload Payload)
{
	OnCombatEvent.Broadcast(Payload);
	UE_LOG(LogTemp, Warning, TEXT("Combat Event Broadcasted."));
}
