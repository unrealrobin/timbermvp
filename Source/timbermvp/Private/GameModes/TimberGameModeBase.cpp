// Property of Paracosm Industries. Dont use my shit.


#include "GameModes/TimberGameModeBase.h"

#include "BrainComponent.h"
#include "AI/TimberAiControllerBase.h"
#include "Character/TimberSeeda.h"
#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Components/BuildSystem/BuildSystemManagerComponent.h"
#include "Controller/TimberPlayerController.h"
#include "Environment/TimberEnemySpawnLocations.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Environment/LabDoorBase.h"
#include "Kismet/GameplayStatics.h"
#include "SaveSystem/TimberSaveSystem.h"

void ATimberGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	DemoSpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	if (GetWorld())
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATimberEnemySpawnLocations::StaticClass(), 
		TimberEnemySpawnPoints);
	}
	GatherAllSpawnLocation(TimberEnemySpawnPoints);


	/*
	 * When the build system component on the character spawns a building component at some location
	 * the Game Mode will then Redraw the Path Trace to show the player where they can build.
	 */
	if(GetWorld())
	{
		TimberCharacter = Cast<ATimberPlayableCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), 
		ATimberPlayableCharacter::StaticClass()));
		if(TimberCharacter)
		{
			TimberCharacter->BuildSystemManager->RedrawPathTraceHandle.AddDynamic(this, 
			&ATimberGameModeBase::HandleRedrawPathTrace);
		}
	}

	// Initial Wave Broadcast
	CurrentWaveNumberHandle.Broadcast(CurrentWaveNumber);

	/*Getting Seedas Location*/
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATimberSeeda::StaticClass(), ArrayOfSpawnedSeedas);
	if (ArrayOfSpawnedSeedas.Num() > 0)
	{
		SeedaLocation = ArrayOfSpawnedSeedas[0]->GetActorLocation();
	}

	

	/*Subscribing to Player Death Delegate Signature*/
	TimberCharacter->HandlePlayerDeath_DelegateHandle.AddDynamic(this, &ATimberGameModeBase::FreezeAllAICharacters);

	GatherAllLabDoors();
}


/*Wave Spawn System*/
void ATimberGameModeBase::SpawnDynamicWave()
{
	// Sets number of each enemy to Spawn
	ComposeWave();
	
	OpenLabDoors();

	//Start the timer for next enemy to spawn, loops continuously until cleared when all enemies are spawned.
	GetWorld()->GetTimerManager().SetTimer(SpawnIncrementsHandle, this , &ATimberGameModeBase::SpawnNextEnemy, 
	TimeBetweenEnemySpawns, true, 1.0f);
	
}

void ATimberGameModeBase::ComposeWave()
{
	//Randomly set the number of each enemy to spawn based on the current wave number
	WaveComposition.BasicRobotCount = FMath::RandRange(1, (CurrentWaveNumber + 5));
	WaveComposition.MeleeWeaponRobotCount = FMath::RandRange(0, CurrentWaveNumber + 4);
	WaveComposition.RangedWeaponRobotCount = FMath::RandRange(0, CurrentWaveNumber + 4);
	//Add the enemies to spawn to the EnemiesToSpawn array
	for(int i = 0; i < WaveComposition.BasicRobotCount; i++)
	{
		EnemiesToSpawn.Add(BasicRobotEnemyClassName);
	}
	for(int i = 0; i < WaveComposition.MeleeWeaponRobotCount; i++)
	{
		EnemiesToSpawn.Add(MeleeRobotEnemyClassName);
	}
	for(int i = 0; i < WaveComposition.RangedWeaponRobotCount; i++)
	{
		EnemiesToSpawn.Add(RangedRobotEnemyClassName);
	}

	//TODO:: Shuffle the array for enemy randomness
}

void ATimberGameModeBase::SpawnNextEnemy()
{
	//Random number of enemies to Spawn
	int EnemiesToSpawnNow = FMath::RandRange(1, 3);

	//Loop through the number of enemies to spawn
	for(int i = 0; i < EnemiesToSpawnNow; i++)
	{
		//If we have spawned fewer enemies than the total enemies to spawn this wave
		if(TotalEnemiesSpawned < EnemiesToSpawn.Num())
		{
			//Were using the TotalEnemiesSpawned as the index to spawn the next enemy
			//At the beginning it starts at 0 and increments by 1 each time an enemy is spawned
			//This will spawn from the start of the array too the end
			SpawnEnemyAtLocation(EnemiesToSpawn[TotalEnemiesSpawned]);
			TotalEnemiesSpawned++;
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(SpawnIncrementsHandle);
			bAllEnemiesSpawned = true;
			return;
		}
	}
}

void ATimberGameModeBase::SpawnEnemyAtLocation(TSubclassOf<ATimberEnemyCharacter> EnemyClassName)
{
	float RandomLocation = FMath::RandRange(0, EnemySpawnPointLocations.Num() - 1);
	ATimberEnemyCharacter* SpawnedActor = GetWorld()->SpawnActor<ATimberEnemyCharacter>(
		EnemyClassName,
		EnemySpawnPointLocations[RandomLocation],
		FRotator::ZeroRotator, DemoSpawnParameter);

	ArrayOfSpawnedWaveEnemies.Add(SpawnedActor);
}

//Checks if destroyed enemies are the ones spawned by the wave system.
// If so, removes them from the array they are stored in.
// Once they are all removed, the wave is over.
void ATimberGameModeBase::CheckArrayForEnemy(ATimberEnemyCharacter* Enemy)
{
	if (ArrayOfSpawnedWaveEnemies.Contains(Enemy))
	{
		GEngine->AddOnScreenDebugMessage(5, 6.0, FColor::Magenta, "Enemy is in the Array");
		ArrayOfSpawnedWaveEnemies.Remove(Enemy);
		if (ArrayOfSpawnedWaveEnemies.Num() == 0 && bAllEnemiesSpawned == true)
		{
			WaveComplete();
			GEngine->AddOnScreenDebugMessage(6, 5.0, FColor::Orange, "Wave Complete. Timer till next wave started.");
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(5, 6.0, FColor::Magenta, "Enemy is NOT in the Array");
	}
}

// Populated Enemy Spawn Point Locations, An Array of Vectors (locations), with all the Spawn points on the map.
// The Spawn points have been put in by the Game Designer
void ATimberGameModeBase::GatherAllSpawnLocation(TArray<AActor*> SpawnPoints)
{
	for (AActor* ActorSpawnPoints : SpawnPoints)
	{
		FVector SpawnPointLocation = ActorSpawnPoints->GetActorLocation();
		EnemySpawnPointLocations.Add(SpawnPointLocation);
	}
}

void ATimberGameModeBase::GatherAllLabDoors()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALabDoorBase::StaticClass(), ArrayOfLabDoors);
}

void ATimberGameModeBase::HandleRedrawPathTrace()
{
	RedrawPathTrace();
}

void ATimberGameModeBase::SpawnTestWave()
{
	SpawnEnemyAtLocation(BasicRobotEnemyClassName);
	SpawnEnemyAtLocation(RangedRobotEnemyClassName);
	SpawnEnemyAtLocation(MeleeRobotEnemyClassName);
}

void ATimberGameModeBase::IncrementWaveNumber()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::Green, "Wave Number Incremented");
	}
	CurrentWaveNumber++;
}

void ATimberGameModeBase::WaveComplete()
{
	
	bAllEnemiesSpawned = false;
	
	IncrementWaveNumber();
	CurrentWaveNumberHandle.Broadcast(CurrentWaveNumber);

	//Save the Game after the Wave is Complete so the Player can continue from the next wave.
	//TODO::Ensure the timing of this works with Delegate Updates.
	SaveCurrentGame();

	CloseLabDoors();
	
	//Starts a Timer for the next wave to Spawn.
	GetWorld()->GetTimerManager().SetTimer(
		TimeToNextWaveHandle, this, &ATimberGameModeBase::SpawnDynamicWave, DurationBetweenWaves,
		false);
}

//Used to Freeze all AI Characters when the Player Dies.
void ATimberGameModeBase::FreezeAllAICharacters(bool bIsPlayerDead)
{
	TArray<AActor*> ArrayOfAICharacters;
	UGameplayStatics::GetAllActorsOfClass(this, ATimberEnemyCharacter::StaticClass(), ArrayOfAICharacters);

	for (AActor* Character : ArrayOfAICharacters)
	{
		ATimberEnemyCharacter* CharacterEnemy = Cast<ATimberEnemyCharacter>(Character);

		if (CharacterEnemy)
		{
			//Stopping Enemy Movement
			CharacterEnemy->GetCharacterMovement()->StopMovementImmediately();

			//Stopping Enemy AI Tree Logic
			if (CharacterEnemy->GetController())
			{
				ATimberAiControllerBase* AIController = Cast<ATimberAiControllerBase>(CharacterEnemy->GetController());
				if (AIController)
				{
					UBrainComponent* Brain = AIController->BrainComponent;
					if (Brain)
					{
						Brain->StopLogic("Freezing because Player Death");
					}
				}
			}
		}
	}
}

/* Save System*/
void ATimberGameModeBase::SaveCurrentGame()
{
	//Creating an instance of the Save Game Object
	UTimberSaveSystem* SaveGameInstance = Cast<UTimberSaveSystem>(
		UGameplayStatics::CreateSaveGameObject
		(UTimberSaveSystem::StaticClass()));

	SaveBuildingComponentData(SaveGameInstance);
	SaveWaveData(SaveGameInstance);


	//TODO:: Create Dynamic Slot names, User to Input Slot Name or will be populated with Wave Info.
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("Demo Timber Save 1"), 0);

	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 3.0, FColor::Red, "ATimberGameModeBase::SaveCurrentGame() -> Game Saved");
	}
}

void ATimberGameModeBase::SaveBuildingComponentData(UTimberSaveSystem* SaveGameInstance)
{
	if (SaveGameInstance)
	{
		TArray<AActor*> CurrentBuildingComponents;
		UGameplayStatics::GetAllActorsOfClass(
			GetWorld(), ATimberBuildingComponentBase::StaticClass(), CurrentBuildingComponents);

		for (AActor* BuildingComponentActors : CurrentBuildingComponents)
		{
			//Creating the Building Component Struct to pass to the Save System's Building Component Array
			FBuildingComponentData BuildingComponentData;
			BuildingComponentData.BuildingComponentClass = BuildingComponentActors->GetClass();
			BuildingComponentData.BuildingComponentTransform = BuildingComponentActors->GetActorTransform();

			SaveGameInstance->BuildingComponentsArray.Add(BuildingComponentData);
		}
	}
}

void ATimberGameModeBase::SaveWaveData(UTimberSaveSystem* SaveGameInstance)
{
	SaveGameInstance->WaveNumber = CurrentWaveNumber;
	UE_LOG(LogTemp, Warning, TEXT("Saved Current Wave Number: %d"), CurrentWaveNumber);
}

/* Load System*/
void ATimberGameModeBase::LoadGame()
{
	ClearAllWaveEnemies();
	//Needs the Slot Name and the User Index
	UTimberSaveSystem* LoadGameInstance = Cast<UTimberSaveSystem>(
		UGameplayStatics::LoadGameFromSlot(TEXT("Demo Timber Save 1"), 0));
	LoadBuildingComponents(LoadGameInstance);
	LoadWaveData(LoadGameInstance);
	LoadPlayerState();
	SwitchToStandardUI.Execute();
	EnableStandardInputMappingContext.Execute();
}

void ATimberGameModeBase::LoadBuildingComponents(UTimberSaveSystem* LoadGameInstance)
{
	if (LoadGameInstance)
	{
		for (FBuildingComponentData BuildingComponentData : LoadGameInstance->BuildingComponentsArray)
		{
			if (BuildingComponentData.BuildingComponentClass)
			{
				GetWorld()->SpawnActor<ATimberBuildingComponentBase>(
					BuildingComponentData.BuildingComponentClass,
					BuildingComponentData.BuildingComponentTransform.GetLocation(),
					BuildingComponentData.BuildingComponentTransform.GetRotation().Rotator());
			}
		}
	}
}

void ATimberGameModeBase::LoadWaveData(UTimberSaveSystem* LoadGameInstance)
{
	CurrentWaveNumber = LoadGameInstance->WaveNumber;
	UE_LOG(LogTemp, Warning, TEXT("Loaded Current Wave Number: %d"), CurrentWaveNumber);
	CurrentWaveNumberHandle.Broadcast(CurrentWaveNumber);
}

void ATimberGameModeBase::LoadPlayerState()
{
	//Move to Start Location
	ATimberPlayerController* TimberPlayerController = Cast<ATimberPlayerController>(
		UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (TimberPlayerController)
	{
		TimberPlayerController->MovePlayerToStartLocation();
	}

	//Reset Player Health
	if (TimberCharacter)
	{
		TimberCharacter->CurrentHealth = TimberCharacter->MaxHealth;
		TimberCharacter->bIsPlayerDead = false;
	}
}

void ATimberGameModeBase::ClearAllWaveEnemies()
{
	for (ATimberEnemyCharacter* ArrayOfSpawnedWaveEnemy : ArrayOfSpawnedWaveEnemies)
	{
		if (ArrayOfSpawnedWaveEnemy)
		{
			ArrayOfSpawnedWaveEnemy->Destroy();
		}
	}

	ArrayOfSpawnedWaveEnemies.Empty();
}

void ATimberGameModeBase::OpenAllLabDoors()
{
	for(AActor* LabDoors : ArrayOfLabDoors)
	{
		ALabDoorBase* LabDoor = Cast<ALabDoorBase>(LabDoors);
		if (LabDoor)
		{
			GEngine->AddOnScreenDebugMessage(5, 3.0, FColor::Black, "LabDoor Exisits");

			LabDoor->OpenLabDoor(GetWorld()->GetDeltaSeconds());
		}
	}
}

void ATimberGameModeBase::OpenLabDoors()
{
	if(ArrayOfLabDoors.Num() <= 0)
	{
		GatherAllLabDoors();
		OpenAllLabDoors();
	}
	else
	{
		OpenAllLabDoors();
	}
}

void ATimberGameModeBase::CloseAllLabDoors()
{
	for(AActor* LabDoors : ArrayOfLabDoors)
	{
		ALabDoorBase* LabDoor = Cast<ALabDoorBase>(LabDoors);
		if (LabDoor)
		{
			LabDoor->CloseLabDoor(GetWorld()->GetDeltaSeconds());
		}
	}
}

void ATimberGameModeBase::CloseLabDoors()
{
	if(ArrayOfLabDoors.Num() <= 0)
	{
		GatherAllLabDoors();
		CloseAllLabDoors();
	}
	else
	{
		CloseAllLabDoors();
	}
}

