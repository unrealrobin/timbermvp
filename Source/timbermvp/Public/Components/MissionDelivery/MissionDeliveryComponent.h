// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types/MissionEventPayloads/MissionEventPayload.h"
#include "MissionDeliveryComponent.generated.h"


class UMissionViewModel;
class UMissionBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TIMBERMVP_API UMissionDeliveryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMissionDeliveryComponent();

	//Stores a List of Missions - To be Populated in BP's
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Missions")
	UDataAsset* MissionsList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Missions")
	TArray<FGuid> CompletedMissionGuids;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Stores the In Progress Mission
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Missions")
	TObjectPtr<UMissionBase> ActiveMission = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Missions")
	TObjectPtr<UMissionViewModel> MissionViewModel = nullptr;
	
private:
	UFUNCTION()
	void HandleBuildEvent(FMissionEventPayload Payload);

	UFUNCTION()
	void HandleCombatEvent(FMissionEventPayload Payload);
	
	void BindToMissionEventSystems();

	void SetActiveMission();

	void GetMissionViewModel();

	UMissionBase* GetActiveMission();

	void InitializeActiveMission(UMissionBase* NewActiveMission);

	void MarkMissionAsCompleted();

	void CreateObjectivesFromMission();
	
	void HandleCombatObjectiveTags(FGameplayTag EventTag);
	void HandleBuildObjectiveTags(FGameplayTag EventTag);

};
