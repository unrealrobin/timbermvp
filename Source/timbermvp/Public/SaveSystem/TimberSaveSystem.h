// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TimberSaveSystem.generated.h"

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UTimberSaveSystem : public USaveGame
{
	GENERATED_BODY()

public:

	//Save Game Variables Here.

	UPROPERTY(VisibleAnywhere, Category="Wave Data")
	float WaveNumber = 0;
	
};
