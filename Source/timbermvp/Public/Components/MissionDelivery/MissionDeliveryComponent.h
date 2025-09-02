// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types/MissionEventPayloads/MissionEventPayload.h"
#include "MissionDeliveryComponent.generated.h"


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
	TArray<UDataAsset*> MissionsList;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Stores the In Progress Mission
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Missions")
	TObjectPtr<UMissionBase> ActiveMission = nullptr;
	
private:
	UFUNCTION()
	void HandleBuildEvent(FMissionEventPayload MissionEventPayload);

	UFUNCTION()
	void HandleCombatEvent(FMissionEventPayload Payload);

	void BindToMissionViewModel();
	void BindToMissionEventSystems();

};
