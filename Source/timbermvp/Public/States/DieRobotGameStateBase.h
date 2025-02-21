// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DieRobotGameStateBase.generated.h"

UENUM(BlueprintType)
enum class ETutorialState : uint8
{
	Wake1 UMETA(DisplayName = "Wake1"),
	Wake2 UMETA(DisplayName = "Wake2"),
	Wake3 UMETA(DisplayName = "Wake3"),
	Default UMETA(DisplayName = "Default")
};

/**
 * 
 */
UCLASS()
class TIMBERMVP_API ADieRobotGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:

	/*
	 * Bound Classes
	 * TimberGameModeBase
	 * 
	 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTutorialStateChange, ETutorialState, NewState);
	FOnTutorialStateChange OnTutorialStateChange;
	
	ETutorialState TutorialState = ETutorialState::Wake1;

	void ChangeTutorialGameState(ETutorialState NewState);
};
