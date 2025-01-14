// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TimerManager.h"
#include "SaveSystem/TimberSaveSystem.h"
#include "TimberGameModeBase.generated.h"

class ALabDoorBase;
class ATimberEnemyCharacter;
class ATimberEnemySpawnLocations;

USTRUCT()
struct FWaveComposition
{
	GENERATED_BODY()

	int BasicRobotCount;
	int MeleeWeaponRobotCount;
	int RangedWeaponRobotCount;

	
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

	DECLARE_DYNAMIC_DELEGATE(FSwitchToStandardUI);

	DECLARE_DYNAMIC_DELEGATE(FEnableStandardInputMappingContext);

	/* Delegate Handle */
	FCurrentWaveNumberHandle CurrentWaveNumberHandle;
	FSwitchToStandardUI SwitchToStandardUI;
	FEnableStandardInputMappingContext EnableStandardInputMappingContext;

	virtual void BeginPlay() override;

	/* Wave System*/
	//Array of Class names to Spawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Wave Composition")
	TArray<TSubclassOf<ATimberEnemyCharacter>> EnemiesToSpawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Wave Composition")
	int TotalEnemiesSpawned = 0;

	UFUNCTION()
	void SpawnNextEnemy();
	
	void SpawnEnemyAtLocation(TSubclassOf<ATimberEnemyCharacter> EnemyClassName);

	UPROPERTY(VisibleAnywhere, Category="Wave Composition")
	int CurrentWaveNumber = 1;

	UFUNCTION(Category="Wave Composition")
	void SpawnDynamicWave();

	/*Spawns a Single AI mob for Testing/Debugging*/
	UFUNCTION(Category="Wave Composition")
	void SpawnTestWave();
	void IncrementWaveNumber();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ATimberEnemyCharacter*> ArrayOfSpawnedWaveEnemies;

	void CheckArrayForEnemy(ATimberEnemyCharacter* Enemy);
	
	FTimerHandle SpawnIncrementsHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave Composition")
	float TimeBetweenEnemySpawns = 1.f;

	FTimerHandle TimeToNextWaveHandle;

	// Stores the actual time remaining for the next wave to spawn. Used in UI.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int TimeToNextWave = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Wave Composition")
	bool bAllEnemiesSpawned = false;

	/* Seeda Details */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Seeda Info")
	FVector SeedaLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AActor*> ArrayOfSpawnedSeedas;

	/* Save System*/
	UFUNCTION(BlueprintCallable, Category="Save System")
	void SaveCurrentGame();
	void SaveBuildingComponentData(UTimberSaveSystem* SaveGameInstance);
	void SaveWaveData(UTimberSaveSystem* SaveGameInstance);
	void LoadPlayerState();

	/*Load System*/
	UFUNCTION(BlueprintCallable, Category="Save System")
	void LoadGame();
	void LoadBuildingComponents(UTimberSaveSystem* LoadGameInstance);
	void LoadWaveData(UTimberSaveSystem* LoadGameInstance);
	UFUNCTION(BlueprintCallable, Category="Wave Composition")
	void ClearAllWaveEnemies();
	void OpenAllLabDoors();

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> TimberEnemySpawnPoints;
	TArray<FVector> EnemySpawnPointLocations;
	void GatherAllSpawnLocation(TArray<AActor*> SpawnPoints);

	//Default Spawn Parameters
	FActorSpawnParameters DemoSpawnParameter;

	/* SpawnableEnemies */
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATimberEnemyCharacter> GoblinEnemyClassName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATimberEnemyCharacter> GhoulEnemyClassName;*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATimberEnemyCharacter> BasicRobotEnemyClassName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATimberEnemyCharacter> MeleeRobotEnemyClassName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATimberEnemyCharacter> RangedRobotEnemyClassName;

	/* Wave Composition and Spawning */
	FWaveComposition WaveComposition;

	UFUNCTION(Category="Wave Composition")
	void ComposeWave();

	UFUNCTION(Category="Wave Composition")
	void WaveComplete();

	/* Wave Timers*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave Timers")
	float DurationBetweenWaves = 10.f;

	/*Character Freeze*/
	// When the player dies, all AI / NPC characters should freeze in place.
	// No movement, No attacking, No AI logic.
	UFUNCTION()
	void FreezeAllAICharacters(bool bIsPlayerDead);

	/*Handle Doors */
	void OpenLabDoors();
	void CloseAllLabDoors();
	void CloseLabDoors();
	UFUNCTION(BlueprintCallable)
	void GatherAllLabDoors();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lab Doors")
	TArray<AActor*> ArrayOfLabDoors;

	
};
