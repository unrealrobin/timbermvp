// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WaveGameInstanceSubsystem.generated.h"

class ATimberEnemyCharacter;
/**
 * 
 */
UCLASS(Blueprintable)
class TIMBERMVP_API UWaveGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Wave Data")
	UDataTable* WaveCompositionTable;
	

	UFUNCTION()
	void StartWave();

	UFUNCTION()
	void IncrementWave();

	UFUNCTION()
	void SpawnWave();

	UFUNCTION()
	void SpawnEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave Data")
	TArray<ATimberEnemyCharacter*> SpawnedEnemies;
};
