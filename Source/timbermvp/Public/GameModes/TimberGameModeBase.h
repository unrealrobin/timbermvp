// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TimerManager.h"
#include "SaveSystem/TimberSaveSystem.h"
#include "TimberGameModeBase.generated.h"

class ATimberSeeda;
class UWaveGameInstanceSubsystem;
class ATimberPlayableCharacter;
class ALabDoorBase;
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
	DECLARE_DYNAMIC_DELEGATE(FSwitchToStandardUI);
	DECLARE_DYNAMIC_DELEGATE(FEnableStandardInputMappingContext);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterInitialization);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSeedaSpawn, AActor*, Seeda);

	/* Delegate Handle */
	FCurrentWaveNumberHandle CurrentWaveNumberHandle;
	FSwitchToStandardUI SwitchToStandardUI;
	FEnableStandardInputMappingContext EnableStandardInputMappingContext;
	FOnCharacterInitialization OnCharacterInitialization;
	FOnSeedaSpawn OnSeedaSpawn;

	virtual void BeginPlay() override;
	
	void PassDataTableToWaveSubsystem(UDataTable* DataTable);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Wave Data")
	UDataTable* WaveCompositionDataTable;
	void PlayerIsInitialized();
	
	void PathTracer_RedrawDelegateBinding();\
	
	void GatherSeedaData();

	//Character
	UPROPERTY()
	ATimberPlayableCharacter* TimberCharacter = nullptr;
	
	/* Seeda Details */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Seeda")
	TSubclassOf<ATimberSeeda> SeedaClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Seeda Info")
	FVector SeedaLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AActor*> ArrayOfSpawnedSeedas;

	/* Save System*/
	UFUNCTION(BlueprintCallable, Category="Save System")
	void SaveCurrentGame();
	void SaveBuildingComponentData(UTimberSaveSystem* SaveGameInstance);
	void SaveWaveData(UTimberSaveSystem* SaveGameInstance);
	void SavePlayerData(UTimberSaveSystem* SaveGameInstance);
	void SaveSeedaData(UTimberSaveSystem* SaveGameInstance);

	/*Load System*/
	UFUNCTION(BlueprintCallable, Category="Save System")
	void LoadGame();
	void LoadBuildingComponents(UTimberSaveSystem* LoadGameInstance);
	void LoadWaveData(UTimberSaveSystem* LoadGameInstance);
	void LoadPlayerState(UTimberSaveSystem* LoadGameInstance);
	void LoadSeedaData(UTimberSaveSystem* LoadGameInstance);
	
	void OpenAllLabDoors();

	/*Character Freeze*/
	UFUNCTION()
	void FreezeAllAICharacters(bool bIsPlayerDead);

protected:

	UWaveGameInstanceSubsystem* GetWaveGameInstanceSubsystem();

	UFUNCTION()
	void PlayBuildMusic();
	UFUNCTION()
	void PlayAttackMusic();

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
