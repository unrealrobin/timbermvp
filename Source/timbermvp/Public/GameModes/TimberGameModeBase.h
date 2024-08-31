// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TimerManager.h"
#include "SaveSystem/TimberSaveSystem.h"
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
	DECLARE_DYNAMIC_DELEGATE(FSwitchToStandardUI);
	DECLARE_DYNAMIC_DELEGATE(FEnableStandardInputMappingContext);
	
	/* Delegate Handle */
	FCurrentWaveNumberHandle CurrentWaveNumberHandle;
	FSwitchToStandardUI SwitchToStandardUI;
	FEnableStandardInputMappingContext EnableStandardInputMappingContext;
	
	virtual void BeginPlay() override;

	/* Wave System*/
	
	void SpawnEnemyAtLocation(TSubclassOf<ATimberEnemyCharacter> EnemyClassName);
	int CurrentWaveNumber = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int TimeToNextWave = 0;
	
	UFUNCTION(Category="Wave Composition")
	void SpawnDynamicWave();

	/*Spawns a Single AI mob for Testing/Debugging*/
	UFUNCTION(Category="Wave Composition")
	void SpawnTestWave();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ATimberEnemyCharacter*> ArrayOfSpawnedWaveEnemies;

	void CheckArrayForEnemy(ATimberEnemyCharacter* Enemy);
	

	FTimerHandle TimeToNextWaveHandle;

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
