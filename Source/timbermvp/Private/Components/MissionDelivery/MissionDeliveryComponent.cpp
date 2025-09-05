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

	//This to be Called from some other Event, Like the start of a Wave.
	//This only looks through the Mission List and Sets the next available mission as the Active Mission.
	//Should be Called from Wave Start.
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
	
	if (ActiveMission && ActiveMissionState == EMissionState::InProgress && MissionViewModel)
	{
		if (CheckMissionContext(Payload, ActiveMission))
		{
			int CurrentCount = MissionViewModel->GetProgressAmount();
			int ObjectiveGoal = MissionViewModel->GetGoalValue();
			MissionViewModel->SetProgressAmount(CurrentCount + Payload.Count);
			MissionViewModel->SetProgressPercent(static_cast<float>(CurrentCount) / static_cast<float>(ObjectiveGoal));

			//TODO:: Need a Way to update completion of mission.
			// Some goal state that is usable for combat missions.

			if (Payload.Count >= ObjectiveGoal)
			{
				int ProgressAmount = MissionViewModel->GetProgressAmount();
				if (ProgressAmount >= ObjectiveGoal)
				{
					MarkMissionAsCompleted();
				}
			}
		}
	}
}

void UMissionDeliveryComponent::UpdateMissionState(EMissionState NewMissionState)
{
	ActiveMissionState = NewMissionState;
	if (MissionViewModel)
	{
		MissionViewModel->SetActiveMissionState(ActiveMissionState);
	}
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
	if (MissionsListDataAsset)
	{
		const TObjectPtr<UMissionList> MissionList = Cast<UMissionList>(MissionsListDataAsset);
		
		if (!MissionList) return;
		
		for (const TObjectPtr<UMissionBase> Mission: MissionList->Missions)
		{
			//Checks whether the mission has been completed before.
			if (Mission && !CompletedMissionGuids.Contains(Mission->MissionID))
			{
				InitializeActiveMission(Mission);
				
				UE_LOG(LogTemp, Warning, TEXT("Mission Delivery Component - Active Mission Set."));
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
		ActiveMission = NewActiveMission;
		UpdateMissionState(EMissionState::InProgress);
		MissionViewModel->ActiveMissionGuid = NewActiveMission->MissionID;
		MissionViewModel->GenerateMissionObjectiveString(NewActiveMission);
		MissionViewModel->SetMissionTitle(NewActiveMission->MissionTitle.ToString());
		MissionViewModel->SetMissionDescription(NewActiveMission->MissionDescription.ToString());
		MissionViewModel->SetProgressAmount(0.0f);
		MissionViewModel->SetProgressPercent(0.0f);
		MissionViewModel->SetGoalValue(NewActiveMission->NumericMissionGoal);
		
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
		UpdateMissionState(EMissionState::Complete);
		//TODO::Should Show Completed State Widget after change to completion. 
		
		//Storing the Completed Mission for Saving.
		CompletedMissionGuids.Add(ActiveMission->MissionID);
	}
}

void UMissionDeliveryComponent::MarkMissionAsIncomplete()
{
	//This is for when a player fails to complete the mission by the end of the Wave.
	if (ActiveMission)
	{
		UpdateMissionState(EMissionState::Incomplete);
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
			FormCombatObjectivesFromTag(EventTag);
		}
		else if (EventTag.MatchesTag(FGameplayTag::RequestGameplayTag("Event.Build")))
		{
			//EventTag = Event.Build.Placed.Structure || Event.Build.Deleted
			FormBuildObjectiveFromTag(EventTag);
		}
	}
}

void UMissionDeliveryComponent::FormCombatObjectivesFromTag(FGameplayTag EventTag)
{
	//TODO:: Populate View Model with Specific Objectives for Combat Objectives
	//Used During Initialize Active Mission.
	if (MissionViewModel && ActiveMission)
	{
		
	}
}

void UMissionDeliveryComponent::FormBuildObjectiveFromTag(FGameplayTag EventTag)
{
	//TODO:: Populate View Model with Specific Objectives for Build Objectives
	if (MissionViewModel && ActiveMission)
	{
		
	}
}

bool UMissionDeliveryComponent::CheckMissionContext(FMissionEventPayload& Payload,
	TWeakObjectPtr<UMissionBase> ActiveMissionRef)
{
	FGameplayTag& EventTag = Payload.EventTag;
	FGameplayTag& MissionTag = ActiveMissionRef->MissionEventType;
	FGameplayTagContainer& EventContextTags = Payload.ContextTags;
	FGameplayTagContainer& MissionContextTags = ActiveMissionRef->RequiredContextTags;

	//Event Context Tag = Event.Enemy.Carbonite.Grunt
	//Mission RequiredContext Tag = Event.Enemy.Carbonite
	if (EventTag.MatchesTagExact(MissionTag) && EventContextTags.HasAll(MissionContextTags) )
	{
		return true;
	}
	
	return false;
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


