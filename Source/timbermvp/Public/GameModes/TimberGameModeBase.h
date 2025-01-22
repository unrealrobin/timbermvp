// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TimerManager.h"
#include "SaveSystem/TimberSaveSystem.h"
#include "TimberGameModeBase.generated.h"

class UWaveGameInstanceSubsystem;
class ATimberPlayableCharacter;
class ALabDoorBase;
class ATimberEnemyCharacter;
class ATimberEnemySpawnLocations;

/*//TODO:: Delete after full implementation in Wave Subsystem
USTRUCT()
struct FWaveComposition
{
	GENERATED_BODY()

	int BasicRobotCount;
	int MeleeWeaponRobotCount;
	int RangedWeaponRobotCount;
};*/

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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterInitialization);

	/* Delegate Handle */
	FCurrentWaveNumberHandle CurrentWaveNumberHandle;
	FSwitchToStandardUI SwitchToStandardUI;
	FEnableStandardInputMappingContext EnableStandardInputMappingContext;
	FOnCharacterInitialization OnCharacterInitialization;

	void PassDataTableToWaveSubsystem(UDataTable* DataTable);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Wave Data")
	UDataTable* WaveCompositionDataTable;
	void PlayerIsInitialized();
	void PathTracer_RedrawDelegateBinding();
	void StoreSeedasLocation();
	virtual void BeginPlay() override;

	//Character
	UPROPERTY()
	ATimberPlayableCharacter* TimberCharacter = nullptr;
	//-----------------------------------------------------------------
	/*/* Wave System#1#
	//Array of Class names to Spawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Wave Composition")
	TArray<TSubclassOf<ATimberEnemyCharacter>> EnemiesToSpawn;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Wave Composition")
	int TotalEnemiesSpawned = 0;
	UFUNCTION()
	void SpawnNextEnemy();
	void SpawnEnemyAtLocation(TSubclassOf<ATimberEnemyCharacter> EnemyClassName);
	void CheckArrayForEnemy(ATimberEnemyCharacter* Enemy);
	UFUNCTION(BlueprintCallable, Category="Wave Composition")
	void ClearAllWaveEnemies();
	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> TimberEnemySpawnPoints;
	TArray<FVector> EnemySpawnPointLocations;
	void GatherAllSpawnLocation(TArray<AActor*> SpawnPoints);
	UPROPERTY(VisibleAnywhere, Category="Wave Composition")
	int CurrentWaveNumber = 1;
	UFUNCTION(Category="Wave Composition")
	void SpawnDynamicWave();
	/*Spawns a Single AI mob for Testing/Debugging#1#
	UFUNCTION(Category="Wave Composition")
	void SpawnTestWave();
	void IncrementWaveNumber();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ATimberEnemyCharacter*> ArrayOfSpawnedWaveEnemies;
	FTimerHandle SpawnIncrementsHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave Composition")
	float TimeBetweenEnemySpawns = 1.f;
	FTimerHandle TimeToNextWaveHandle;
	// Stores the actual time remaining for the next wave to spawn. Used in UI.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int TimeToNextWave = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Wave Composition")
	bool bAllEnemiesSpawned = false;
	//Default Spawn Parameters
	FActorSpawnParameters DemoSpawnParameter;
	/* SpawnableEnemies #1#
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATimberEnemyCharacter> GoblinEnemyClassName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATimberEnemyCharacter> GhoulEnemyClassName;#1#
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATimberEnemyCharacter> BasicRobotEnemyClassName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATimberEnemyCharacter> MeleeRobotEnemyClassName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATimberEnemyCharacter> RangedRobotEnemyClassName;
	/* Wave Composition and Spawning #1#
	FWaveComposition WaveComposition;
	UFUNCTION(Category="Wave Composition")
	void ComposeWave();
	UFUNCTION(Category="Wave Composition")
	void WaveComplete();
	/* Wave Timers#1#
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave Timers")
	float DurationBetweenWaves = 10.f;*/

	//-----------------------------------------------------------------


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
	
	void OpenAllLabDoors();

protected:

	UWaveGameInstanceSubsystem* GetWaveGameInstanceSubsystem();

	
	

	
	/*Character Freeze*/
	// When the player dies, all AI / NPC characters should freeze in place.
	// No movement, No attacking, No AI logic.
	UFUNCTION()
	void FreezeAllAICharacters(bool bIsPlayerDead);

	/*Handle Doors */
	UFUNCTION()
	void OpenLabDoors();
	void CloseAllLabDoors();
	UFUNCTION()
	void CloseLabDoors();
	UFUNCTION(BlueprintCallable)
	void GatherAllLabDoors();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lab Doors")
	TArray<AActor*> ArrayOfLabDoors;

	/* Path Tracing */
	UFUNCTION(BlueprintImplementableEvent)
	void RedrawPathTrace();
	UFUNCTION()
	void HandleRedrawPathTrace();
};
