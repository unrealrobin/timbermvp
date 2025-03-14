// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DieRobotGameStateBase.generated.h"

class ARecastNavMesh;

UENUM(BlueprintType)
enum class EGameState : uint8
{
	MainMenu UMETA(DisplayName = "MainMenu"),
	MidGameDemo UMETA(DisplayName = "MidGameDemo"),
	Standard UMETA(DisplayName = "MidGameDemo"),
	Default UMETA(DisplayName = "Default"),
	
};

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
	TutorialComplete UMETA(DisplayName = "TutorialCompleted"),
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
	 * Controller
	 * HUD
	 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTutorialStateChange, ETutorialState, NewState);
	FOnTutorialStateChange OnTutorialStateChange;

	UPROPERTY(VisibleAnywhere, Category = "State")
	EGameState CurrentGameState = EGameState::Default;

	UPROPERTY(VisibleAnywhere, Category = "State")
	//Tutorial State when Starting Game With Tutorial
	ETutorialState TutorialState = ETutorialState::Wake1;

	//Tutorial state when starting Game outside of Tutorial
	//ETutorialState TutorialState = ETutorialState::TutorialComplete;

	void ChangeTutorialGameState(ETutorialState NewState);
	
};
