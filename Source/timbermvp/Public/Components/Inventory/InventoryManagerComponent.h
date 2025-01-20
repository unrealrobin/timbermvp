// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "States/PlayerStateBase.h"
#include "InventoryManagerComponent.generated.h"


struct FBuildableCost;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TIMBERMVP_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryManagerComponent();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateInventoryHandle);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void LoadOwningPlayerState();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player State")
	APlayerStateBase* PS = nullptr;

	UFUNCTION()
	int GetPartsInInventory();

	UFUNCTION()
	void AddPartsToInventory(int PartsToAdd);

	UFUNCTION()
	void RemovePartsFromInventory(int PartsToRemove);

	UFUNCTION()
	bool bCanAffordCost(FBuildableCost CostOfBuildable);

	UPROPERTY(BlueprintAssignable, Category="Inventory Delegate Handle")
	FUpdateInventoryHandle UpdateInventoryHandle;
};
