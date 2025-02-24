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
	Combat1 UMETA(DisplayName = "Combat1"),
	Combat2 UMETA(DisplayName = "Combat2"),
	Parts1 UMETA(DisplayName = "Parts1"),
	Building1 UMETA(DisplayName = "Building1"),
	Building2 UMETA(DisplayName = "Building2"),
	Building3 UMETA(DisplayName = "Building3"),
	WaveStart UMETA(DisplayName = "WaveStart"),
	WaveComplete UMETA(DisplayName = "WaveComplete"),
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
