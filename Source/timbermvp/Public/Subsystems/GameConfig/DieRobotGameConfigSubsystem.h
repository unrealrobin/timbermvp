// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DieRobotGameConfigSubsystem.generated.h"

UENUM(BlueprintType)
enum class EDieRobotGameConfigType : uint8
{
	MainMenu UMETA(DisplayName = "MainMenu"),
	MidGameDemo UMETA(DisplayName = "MidGameDemo"),
	Standard UMETA(DisplayName = "MidGameDemo"),
	Default UMETA(DisplayName = "Default"),
};

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UDieRobotGameConfigSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	EDieRobotGameConfigType GameConfig = EDieRobotGameConfigType::Default;
};
