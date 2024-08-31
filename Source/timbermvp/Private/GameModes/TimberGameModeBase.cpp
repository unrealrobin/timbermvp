// Property of Paracosm Industries. Dont use my shit.


#include "GameModes/TimberGameModeBase.h"

#include "Character/TimberSeeda.h"
#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Controller/TimberPlayerController.h"
#include "Environment/TimberEnemySpawnLocations.h"
#include "Kismet/GameplayStatics.h"
#include "SaveSystem/TimberSaveSystem.h"

void ATimberGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	DemoSpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	UWorld* World = GetWorld();
	if(World)
	{
		UGameplayStatics::GetAllActorsOfClass(World, ATimberEnemySpawnLocations::StaticClass(), TimberEnemySpawnPoints);
	}
	GatherAllSpawnLocation(TimberEnemySpawnPoints);

	// Initial Wave Broadcast
	CurrentWaveNumberHandle.Broadcast(CurrentWaveNumber);

	/*Getting Seedas Location*/
	UGameplayStatics::GetAllActorsOfClass(World, ATimberSeeda::StaticClass(), ArrayOfSpawnedSeedas);
	SeedaLocation = ArrayOfSpawnedSeedas[0]->GetActorLocation();
	
}

//Checks if destroyed enemies are the ones spawned by the wave system.
// If so, removes them from the array they are stored in.
// Once they are all removed, the wave is over.
void ATimberGameModeBase::CheckArrayForEnemy(ATimberEnemyCharacter* Enemy)
{
	if(ArrayOfSpawnedWaveEnemies.Contains(Enemy))
	{
		GEngine->AddOnScreenDebugMessage(5, 6.0, FColor::Magenta, "Enemy is in the Array");
		ArrayOfSpawnedWaveEnemies.Remove(Enemy);
		if(ArrayOfSpawnedWaveEnemies.Num() == 0)
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
	for (AActor* ActorSpawnPoints : SpawnPoints) {
		FVector SpawnPointLocation = ActorSpawnPoints->GetActorLocation();
		EnemySpawnPointLocations.Add(SpawnPointLocation);
	}
}

void ATimberGameModeBase::ComposeWave()
{
	
	Wave.GhoulCount = FMath::RandRange(1, (CurrentWaveNumber + 4));
	
	Wave.GoblinCount = FMath::RandRange(0, CurrentWaveNumber + 2);
}

void ATimberGameModeBase::SpawnEnemyAtLocation(TSubclassOf<ATimberEnemyCharacter> EnemyClassName)
{
	float RandomLocation = FMath::RandRange(0, EnemySpawnPointLocations.Num() - 1);
	ATimberEnemyCharacter* SpawnedActor = GetWorld()->SpawnActor<ATimberEnemyCharacter>(EnemyClassName, 
	                                              EnemySpawnPointLocations[RandomLocation],
	                                              FRotator::ZeroRotator, DemoSpawnParameter);

	ArrayOfSpawnedWaveEnemies.Add(SpawnedActor);
	
}

void ATimberGameModeBase::SpawnDynamicWave()
{
	ComposeWave();
	
	//Spawn Ghouls
	for(int i = 0; i < Wave.GhoulCount; i++)
	{
		SpawnEnemyAtLocation(GhoulEnemyClassName);
	}
	
	//Spawn Goblins
	for(int i = 0; i < Wave.GoblinCount; i++)
	{
		SpawnEnemyAtLocation(GoblinEnemyClassName);
	}
}

void ATimberGameModeBase::SpawnTestWave()
{
	
		SpawnEnemyAtLocation(GhoulEnemyClassName);
		SpawnEnemyAtLocation(GoblinEnemyClassName);
	
}

void ATimberGameModeBase::IncrementWaveNumber()
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::Green, "Wave Number Incremented");
	}
	CurrentWaveNumber++;
}

void ATimberGameModeBase::WaveComplete()
{
	
	IncrementWaveNumber();
	CurrentWaveNumberHandle.Broadcast(CurrentWaveNumber);

	//Save the Game after the Wave is Complete so the Player can continue from the next wave.
	//TODO::Ensure the timing of this works with Delegate Updates.
	SaveCurrentGame();

	//Starts a Timer for the next wave to Spawn.
	GetWorld()->GetTimerManager().SetTimer(TimeToNextWaveHandle,this, &ATimberGameModeBase::SpawnDynamicWave, DurationBetweenWaves, 
	false);
	
}

/* Save System*/

void ATimberGameModeBase::SaveCurrentGame()
{
	//Creating an instance of the Save Game Object
	UTimberSaveSystem* SaveGameInstance = Cast<UTimberSaveSystem>(UGameplayStatics::CreateSaveGameObject
		(UTimberSaveSystem::StaticClass()));
	
	SaveBuildingComponentData(SaveGameInstance);
	SaveWaveData(SaveGameInstance);


	//TODO:: Create Dynamic Slot names, User to Input Slot Name or will be populated with Wave Info.
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("Demo Timber Save 1"), 0);
}

void ATimberGameModeBase::SaveBuildingComponentData(UTimberSaveSystem* SaveGameInstance)
{
	if(SaveGameInstance)
	{
		TArray<AActor*> CurrentBuildingComponents;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATimberBuildingComponentBase::StaticClass(), CurrentBuildingComponents);
		
		for(AActor* BuildingComponentActors : CurrentBuildingComponents)
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
	UTimberSaveSystem* LoadGameInstance = Cast<UTimberSaveSystem>(UGameplayStatics::LoadGameFromSlot(TEXT("Demo Timber Save 1"), 0));
	LoadBuildingComponents(LoadGameInstance);
	LoadWaveData(LoadGameInstance);
	LoadPlayerState();
	SwitchToStandardUI.Execute();
	EnableStandardInputMappingContext.Execute();
}


void ATimberGameModeBase::LoadBuildingComponents(UTimberSaveSystem* LoadGameInstance)
{
	if(LoadGameInstance)
	{
		for(FBuildingComponentData BuildingComponentData : LoadGameInstance->BuildingComponentsArray)
		{
			if(BuildingComponentData.BuildingComponentClass)
			{
				GetWorld()->SpawnActor<ATimberBuildingComponentBase>(BuildingComponentData.BuildingComponentClass, 
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
	ATimberPlayerController* TimberPlayerController = Cast<ATimberPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if(TimberPlayerController)
	{
		TimberPlayerController->MovePlayerToStartLocation();
	}

	//Reset Player Health
	ATimberPlayableCharacter* TimberCharacter = Cast<ATimberPlayableCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if(TimberCharacter)
	{
		TimberCharacter->CurrentHealth = TimberCharacter->MaxHealth;
	}
}

void ATimberGameModeBase::ClearAllWaveEnemies()
{
	//TODO:: Remove all enemies from the map.
	//Typically used when loading a game after death when enemies are still on the map.
	
	for (ATimberEnemyCharacter* ArrayOfSpawnedWaveEnemy : ArrayOfSpawnedWaveEnemies)
	{
		if(ArrayOfSpawnedWaveEnemy)
		{
			ArrayOfSpawnedWaveEnemy->Destroy();
		}
	}

	ArrayOfSpawnedWaveEnemies.Empty();
}



