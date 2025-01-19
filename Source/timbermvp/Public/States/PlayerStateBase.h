// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerStateBase.generated.h"

USTRUCT(BlueprintType)
struct FInventory
{
	GENERATED_BODY()

public:

	/* Parts used for Building Components and Trap */
	UPROPERTY(VisibleAnywhere)
	int NumberOfParts = 0;
};


/**
 * 
 */
UCLASS()
class TIMBERMVP_API APlayerStateBase : public APlayerState
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Main Inventory")
	FInventory MainInventory;
	
};
