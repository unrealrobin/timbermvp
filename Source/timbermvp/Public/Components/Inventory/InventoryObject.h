// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InventoryObject.generated.h"

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UInventoryObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory Items")
	int NumberOfParts = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory Items")
	int NumberOfMechanism = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory Items")
	int NumberOfUniques = 0;
};
