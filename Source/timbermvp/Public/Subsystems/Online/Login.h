// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Login.generated.h"

/**
 * 
 */
UCLASS()
class TIMBERMVP_API ULogin : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	void Initialize(FSubsystemCollectionBase& Collection) override;
};
