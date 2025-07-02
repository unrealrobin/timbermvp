// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DieRobotGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UDieRobotGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void Init() override;
};
