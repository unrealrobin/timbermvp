// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TimberGameModeBase.generated.h"

class ATimberEnemyCharacter;
class ATimberEnemySpawnLocations;

/**
 * 
 */
UCLASS()
class TIMBERMVP_API ATimberGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	/* Delegate Signature*/
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCurrentWaveNumberHandle, float, CurrentWaveNumber);

	/* Delegate Handle */
	FCurrentWaveNumberHandle CurrentWaveNumberHandle;
	
	virtual void BeginPlay() override;

	//TODO: Remove after spawning system built.
	void SpawnWave(TArray<TSubclassOf<ATimberEnemyCharacter>> EnemiesToSpawn);
	
	float CurrentWaveNumber = 1;

protected:

	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> TimberEnemySpawnPoints;
	
	TArray<FVector> EnemySpawnPointLocations;
	
	void GatherAllSpawnLocation(TArray<AActor*> SpawnPoints);

	//TODO: Remove after spawning system built.
	UPROPERTY(EditAnywhere, Category="Wave Spawn")
	int NumberOfEnemiesToSpawn = 2;

	//Default Spawn Parameters
	FActorSpawnParameters DemoSpawnParameter;

	/* SpawnableEnemies */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATimberEnemyCharacter> GoblinEnemyClassName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATimberEnemyCharacter> GhoulEnemyClassName;
};
