// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TimerManager.h"
#include "SaveSystem/TimberSaveSystem.h"
#include "States/DieRobotGameStateBase.h"
#include "TimberGameModeBase.generated.h"

class ALocationMarkerBase;
enum class ETutorialState : uint8;
enum class EDieRobotGameConfigType : uint8;
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

	/* Game State / Tutorial */
	void InitializeGameState();

	UFUNCTION()
	void HandleTutorialStateChange(ETutorialState NewState);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Game State")
	ETutorialState TutorialState = ETutorialState::Default;
	
	void GetTutorialState();
	
	UFUNCTION()
	void UpdateTutorialState(ETutorialState NewState);

	UFUNCTION()
	void SpawnDummyForTutorial();

	/*Character*/
	UPROPERTY()
	ATimberPlayableCharacter* TimberCharacter = nullptr;
	
	/* Seeda Details */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Seeda")
	TSubclassOf<ATimberSeeda> SeedaClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Seeda Info")
	FVector SeedaLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AActor*> ArrayOfSpawnedSeedas;

	/*Save/Load Slots*/
	FString DEVELOPER_SAVE_BASE_MIDGAME = TEXT("DEVELOPER_BASE_SAVE");
	FString MidGameDemoSaveSlot = TEXT("MidGameDemoSaveSlot");
	FString StandardSaveSlot = TEXT("StandardSaveSlot");

	UFUNCTION()
	FString GetSaveSlot();
	
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

	UFUNCTION()
	void HandleWaveComplete(int CompletedWave);
	
	void OpenAllLabDoors();

	/*Character Freeze*/
	UFUNCTION()
	void FreezeAllAICharacters(bool bIsPlayerDead);

	/* Location Marker */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TSubclassOf<ALocationMarkerBase> LocationMarker;

	UFUNCTION(BlueprintCallable)
	void SwitchToMainMenu();

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

	/*Spawn*/

	UFUNCTION()
	void SpawnLocationMarker();
};
