// Property of Paracosm.


#include "Components/MissionDelivery/MissionDeliveryComponent.h"

#include "MVVMGameSubsystem.h"
#include "MVVMViewModelBase.h"
#include "Data/DataAssets/MissionSystem/MissionBase.h"
#include "Data/DataAssets/MissionSystem/MissionList.h"
#include "Subsystems/Events/BuildEventSubsystem.h"
#include "Subsystems/Events/CombatEventSubsystem.h"
#include "ViewModels/MissionViewModel.h"


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
	GetMissionViewModel();
	SetActiveMission();
}

void UMissionDeliveryComponent::HandleBuildEvent(FMissionEventPayload Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("MDC - Handling Build Event."));
}

void UMissionDeliveryComponent::HandleCombatEvent(FMissionEventPayload Payload)
{
	//Pushes all Damage Event Info to the View Model IF Active Mission Event Matches Payload Event.
	UE_LOG(LogTemp, Warning, TEXT("MDC - Handling Combat Event."));
	
	
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

void UMissionDeliveryComponent::SetActiveMission()
{
	if (MissionsList)
	{
		UMissionList* ML = Cast<UMissionList>(MissionsList);
		if (!ML) return;
		
		for (UMissionBase* Mission: ML->Missions)
		{
			UMissionBase* M = Cast<UMissionBase>(Mission);
			
			if (M && !CompletedMissionGuids.Contains(M->MissionID))
			{
				ActiveMission = M;
				ActiveMission->MissionState = EMissionState::InProgress;
				UE_LOG(LogTemp, Warning, TEXT("Mission Delivery Component - Active Mission Set."));
				InitializeActiveMission(ActiveMission);
				break;
			}
		}
	}
}

UMissionBase* UMissionDeliveryComponent::GetActiveMission()
{
	return ActiveMission;
}

void UMissionDeliveryComponent::InitializeActiveMission(UMissionBase* NewActiveMission)
{
	if (NewActiveMission && MissionViewModel)
	{
		MissionViewModel->GenerateMissionObjectiveString(NewActiveMission);
		MissionViewModel->SetMissionTitle(NewActiveMission->MissionTitle.ToString());
		MissionViewModel->SetMissionDescription(NewActiveMission->MissionDescription.ToString());
		MissionViewModel->SetProgressAmount(0.0f);
		MissionViewModel->SetProgressPercent(0.0f);
		MissionViewModel->SetActiveMissionState(EMissionState::InProgress);
		UE_LOG(LogTemp, Warning, TEXT("Mission Delivery Component - Active Mission Initialized."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Mission Delivery Component - Could Not Initialize Active Mission."));
	}
}

void UMissionDeliveryComponent::MarkMissionAsCompleted()
{
	if (ActiveMission)
	{
		//We can Signify Completion Status through the UI.
		MissionViewModel->SetActiveMissionState(EMissionState::Complete);

		//Storing the Completed Mission for Saving.
		CompletedMissionGuids.Add(ActiveMission->MissionID);
	}
}

void UMissionDeliveryComponent::CreateObjectivesFromMission()
{
	if (ActiveMission)
	{
		FGameplayTag EventTag = ActiveMission->MissionEventType;

		if (EventTag.MatchesTag(FGameplayTag::RequestGameplayTag("Event.Combat")))
		{
			// EventTag = Event.Combat.Destroy || Event.Combat.Damage
			HandleCombatObjectiveTags(EventTag);
		}
		else if (EventTag.MatchesTag(FGameplayTag::RequestGameplayTag("Event.Build")))
		{
			//EventTag = Event.Build.Placed.Structure || Event.Build.Deleted
			HandleBuildObjectiveTags(EventTag);
		}
	}
}

void UMissionDeliveryComponent::HandleCombatObjectiveTags(FGameplayTag EventTag)
{
	//TODO:: Populate View Model with Specific Objectives for Combat Objectives
	//Used During Initialize Active Mission.
	if (MissionViewModel && ActiveMission)
	{
		
	}
}

void UMissionDeliveryComponent::HandleBuildObjectiveTags(FGameplayTag EventTag)
{
	//TODO:: Populate View Model with Specific Objectives for Build Objectives
	if (MissionViewModel && ActiveMission)
	{
		
	}
}

void UMissionDeliveryComponent::GetMissionViewModel()
{
	TObjectPtr<UMVVMGameSubsystem> MVGS = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>();
	if (IsValid(MVGS))
	{
		TObjectPtr<UMVVMViewModelCollectionObject> Collection = MVGS->GetViewModelCollection();
		if (IsValid(Collection))
		{
			FMVVMViewModelContext Context;
			Context.ContextClass = UMissionViewModel::StaticClass();
			Context.ContextName = "MissionVM";
			
			TObjectPtr<UMVVMViewModelBase> VM_Base = Collection->FindViewModelInstance(Context);
			if (IsValid(VM_Base))
			{
				MissionViewModel = Cast<UMissionViewModel>(VM_Base);
				UE_LOG(LogTemp, Warning, TEXT("Mission Delivery Component - Found the Mission View Model."));
				return;
			}

			UE_LOG(LogTemp, Warning, TEXT("Mission Delivery Component - Could Not Get the Mission View Model from Global Collection."));
		}
	}
}


