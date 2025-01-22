// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WaveGameInstanceSubsystem.generated.h"

class AGarageDoorBase;
class ATimberEnemyCharacter;

/**
 * 
 */
UCLASS(Blueprintable)
class TIMBERMVP_API UWaveGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	/* Delegates */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCurrentWaveHandle, int, CurrentWaveNumber);
	FCurrentWaveHandle CurrentWaveHandle;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOpenLabDoorHandle);
	FOpenLabDoorHandle OpenLabDoorHandle;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCloseLabDoorHandle);
	FOpenLabDoorHandle CloseLabDoorHandle;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSaveCurrentGameHandle);
	FSaveCurrentGameHandle SaveCurrentGameHandle;	

	/* Data Table */
	UFUNCTION()
	void SetWaveCompositionDataTable(UDataTable* DataTable);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Wave Data")
	UDataTable* WaveCompositionTable;

	/* Preparing SpawnPoints for Enemies */
	UFUNCTION()
	void PrepareSpawnPoints();
	UFUNCTION()
	void GatherAllStandardSpawnPoints();
	UFUNCTION()
	void GatherAllStandardSpawnPointLocations();
	UFUNCTION()
	void GetBossSpawnPointLocation();
	UFUNCTION()
	FVector GetRandomStandardSpawnPointLocation();
	UFUNCTION()
	void GetGarageDoor();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn Points")
	TArray<FVector> StandardSpawnPointLocations;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn Points")
	TArray<AActor*> StandardSpawnPoints;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn Points")
	FVector BossSpawnPointLocation;

	/*Wave Spawning and Composition*/
	UFUNCTION()
	void StartWave();
	UFUNCTION()
	void ComposeWaveFromDataTable();
	UFUNCTION()
	void HandleBossSpawn();
	UFUNCTION()
	void SpawnPartOfWave();
	UFUNCTION()
	void SpawnWave();
	UFUNCTION()
	void EndWave();
	UFUNCTION()
	void IncrementWave();
	UFUNCTION()
	void SpawnEnemy(TSubclassOf<AActor> ActorToSpawn, FVector Location);
	UPROPERTY()
	int CurrentWaveNumber = 3;
	UPROPERTY()
	TArray<TSubclassOf<ATimberEnemyCharacter>> EnemiesToSpawn;
	UPROPERTY()
	TSubclassOf<AActor> BossToSpawn = nullptr;
	UPROPERTY()
	int TotalEnemiesToSpawn = 0;
	UPROPERTY()
	int TotalEnemiesSpawned = 0;
	UPROPERTY()
	bool bAllEnemiesSpawned = false;
	FTimerHandle SpawnPartOfWaveTimerHandle;
	UPROPERTY()
	float TimeBetweenEnemySpawns = 5.f;
	
	/*Enemies*/
	UPROPERTY()
	TArray<ATimberEnemyCharacter*> SpawnedEnemies;
	UFUNCTION()
	void CheckArrayForEnemy(ATimberEnemyCharacter* Enemy);
	UFUNCTION()
	void ResetWaveEnemies();
	UPROPERTY()
	bool BossSpawned = false;

	/*Wave Iteration*/
	UPROPERTY()
	int TimeBetweenWaves = 20;

	FTimerHandle TimeToNextWaveHandle;

	/*Boss*/
	UPROPERTY()
	AGarageDoorBase* BossDoor;
	UFUNCTION()
	void HandleBossDoor(bool ShouldDoorBeOpen);
	FTimerHandle BossSpawnDelayTimerHandle;
};

