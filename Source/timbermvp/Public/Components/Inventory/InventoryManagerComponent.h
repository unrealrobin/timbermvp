// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "States/PlayerStateBase.h"
#include "InventoryManagerComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TIMBERMVP_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player State")
	APlayerStateBase* PS;

public:
	// Called every frame
	virtual void TickComponent(
		float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	int GetPartsInInventory();

	UFUNCTION()
	void AddPartsToInventory(int PartsToAdd);

	UFUNCTION()
	void RemovePartsFromInventory(int PartsToRemove);

	bool bCanAffordPartsCost(int CostOfParts);
};
