// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/DataAssets/MissionSystem/MissionBase.h"
#include "Types/MissionEventPayloads/MissionEventPayload.h"
#include "MissionDeliveryComponent.generated.h"

class UMissionViewModel;
class UMissionBase;

UENUM(BlueprintType) 
enum class EMissionState : uint8
{
	Incomplete UMETA(DisplayName = "Not Complete"),
	InProgress UMETA(DisplayName = "In Progress"),
	Complete UMETA(DisplayName = "Complete"),
	Default UMETA(DisplayName = "Default")
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TIMBERMVP_API UMissionDeliveryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMissionDeliveryComponent();
	
	void BindToWaveSubsystem();

	//Stores a List of Missions - To be Populated in BP's
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Missions")
	TObjectPtr<UDataAsset> MissionsListDataAsset = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Missions")
	TArray<FGuid> CompletedMissionGuids;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Missions")
	EMissionState ActiveMissionState = EMissionState::Default;
	
protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;

	//Stores the In Progress Mission
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Missions")
	TObjectPtr<UMissionBase> ActiveMission = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Missions")
	TObjectPtr<UMissionViewModel> MissionViewModel = nullptr;
	
private:
	/* Delegate Callbacks*/
	UFUNCTION()
	void HandleBuildEvent(FMissionEventPayload Payload);
	UFUNCTION()
	void HandleCombatEvent(FMissionEventPayload Payload);
	UFUNCTION()
	void HandleWaveStart(int CompletedWaveNumber);
	UFUNCTION()
	void HandleWaveEnd(int CompletedWaveNumber);
	void ResetMissionViewModel();
	void PlayMissionDialogue();
	void UpdateMissionState(EMissionState NewMissionState);
	void BindToMissionEventSystems();
	void SetActiveMission();
	void GetMissionViewModel();
	void InitializeActiveMission(UMissionBase* NewActiveMission);
	void MarkMissionAsCompleted();
	void MarkMissionAsIncomplete();
	void CreateObjectivesFromMission();
	void FormCombatObjectivesFromTag(FGameplayTag EventTag);
	void FormBuildObjectiveFromTag(FGameplayTag EventTag);
	bool CheckMissionContext(FMissionEventPayload& Payload, TWeakObjectPtr<UMissionBase> ActiveMissionRef);
	UMissionBase* GetActiveMission();

	UPROPERTY()
	int CurrentWaveNumber = 0;

	
};
