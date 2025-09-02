// Property of Paracosm.


#include "Components/MissionDelivery/MissionDeliveryComponent.h"

#include "MVVMViewModelBase.h"
#include "Subsystems/Events/BuildEventSubsystem.h"
#include "Subsystems/Events/CombatEventSubsystem.h"


// Sets default values for this component's properties
UMissionDeliveryComponent::UMissionDeliveryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UMissionDeliveryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	BindToMissionEventSystems();
}

void UMissionDeliveryComponent::HandleBuildEvent(FMissionEventPayload MissionEventPayload)
{
	UE_LOG(LogTemp, Warning, TEXT("MDC - Handling Build Event."));
}

void UMissionDeliveryComponent::HandleCombatEvent(FMissionEventPayload Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("MDC - Handling Combat Event."));
	
}

void UMissionDeliveryComponent::BindToMissionViewModel()
{
	
}

void UMissionDeliveryComponent::BindToMissionEventSystems()
{
	UCombatEventSubsystem* CES = GetWorld()->GetGameInstance()->GetSubsystem<UCombatEventSubsystem>();
	UBuildEventSubsystem* BES = GetWorld()->GetGameInstance()->GetSubsystem<UBuildEventSubsystem>();

	if (!BES || !CES) return;
	
	BES->OnBuildEvent.AddDynamic(this, &UMissionDeliveryComponent::HandleBuildEvent);
	UE_LOG(LogTemp, Warning, TEXT("Bound to Build Event Subsystem."));
	
	CES->OnCombatEvent.AddDynamic(this, &UMissionDeliveryComponent::HandleCombatEvent);
	UE_LOG(LogTemp, Warning, TEXT("Bound to Combat Event Subsystem."));
	
}

