// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TimerManager.h"
//#include "Engine/World.h"
#include "TimberGameModeBase.generated.h"

class ATimberEnemyCharacter;
class ATimberEnemySpawnLocations;

USTRUCT()
struct FWaveComposition
{
	GENERATED_BODY()

	int GoblinCount;
	int GhoulCount;
	
	
};

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
	
	void SpawnEnemyAtLocation(TSubclassOf<ATimberEnemyCharacter> EnemyClassName);
	int CurrentWaveNumber = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int TimeToNextWave = 0;

	//Here for testing purposes
	UFUNCTION(Category="Wave Composition")
	void SpawnDynamicWave();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ATimberEnemyCharacter*> ArrayOfSpawnedWaveEnemies;

	void CheckArrayForEnemy(ATimberEnemyCharacter* Enemy);

	FTimerHandle TimeToNextWaveHandle;

protected:

	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> TimberEnemySpawnPoints;
	
	TArray<FVector> EnemySpawnPointLocations;
	
	void GatherAllSpawnLocation(TArray<AActor*> SpawnPoints);

	//Default Spawn Parameters
	FActorSpawnParameters DemoSpawnParameter;

	/* SpawnableEnemies */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATimberEnemyCharacter> GoblinEnemyClassName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATimberEnemyCharacter> GhoulEnemyClassName;

	/* Wave Composition and Spawning */
	FWaveComposition Wave;

	UFUNCTION(Category="Wave Composition")
	void ComposeWave();
	
	UFUNCTION(Category="Wave Composition")
	void WaveComplete();

	/* Wave Timers*/
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave Timers")
	float DurationBetweenWaves = 10.f;
	
	
};
