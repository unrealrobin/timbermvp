// Property of Paracosm Industries. Dont use my shit.


#include "Subsystems/Wave/WaveGameInstanceSubsystem.h"

#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Data/WaveData.h"
#include "Environment/BossSpawnLocation.h"
#include "Environment/GarageDoorBase.h"
#include "Environment/TimberEnemySpawnLocations.h"
#include "Kismet/GameplayStatics.h"


void UWaveGameInstanceSubsystem::SetWaveCompositionDataTable(UDataTable* DataTable)
{
	WaveCompositionTable = DataTable;
}

void UWaveGameInstanceSubsystem::PrepareSpawnPoints()
{
	GatherAllStandardSpawnPoints();
	GatherAllStandardSpawnPointLocations();
	GetBossSpawnPointLocation();
	GetGarageDoor();
}

void UWaveGameInstanceSubsystem::GatherAllStandardSpawnPoints()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATimberEnemySpawnLocations::StaticClass(), StandardSpawnPoints);
}

void UWaveGameInstanceSubsystem::GatherAllStandardSpawnPointLocations()
{
	if(StandardSpawnPoints.Num() > 0)
	{
		for(AActor* SpawnPoints : StandardSpawnPoints)
		{
			StandardSpawnPointLocations.Add(SpawnPoints->GetActorLocation());
		}
	}
}

void UWaveGameInstanceSubsystem::GetBossSpawnPointLocation()
{
	BossSpawnPointLocation = UGameplayStatics::GetActorOfClass(GetWorld(), ABossSpawnLocation::StaticClass())
	->GetActorLocation();
}

FVector UWaveGameInstanceSubsystem::GetRandomStandardSpawnPointLocation()
{
	if(StandardSpawnPointLocations.Num() > 0)
	{
		return StandardSpawnPointLocations[FMath::RandRange(0, StandardSpawnPointLocations.Num() - 1)];
	}

	return FVector::ZeroVector;
}

void UWaveGameInstanceSubsystem::GetGarageDoor()
{
	BossDoor = Cast<AGarageDoorBase>(UGameplayStatics::GetActorOfClass(GetWorld(), AGarageDoorBase::StaticClass()));
}

void UWaveGameInstanceSubsystem::StartWave()
{
	if(CurrentWaveNumber >= 30)
	{
		UE_LOG(LogTemp, Warning, TEXT("Its about to get wild. Endless mode. Bucket of popcorn time. Leaderboards in view."));
	}
	else
	{
		//Prepares the Wave Composition
		ComposeWaveFromDataTable();
		//Process of Opening Doors
		OpenLabDoorHandle.Broadcast();
		//Spawning Enemies from Data Tables
		SpawnWave();
	}
	
}

void UWaveGameInstanceSubsystem::ComposeWaveFromDataTable()
{
	//Searches the data tables for the current wave number and then spawns the enemies
	if(!WaveCompositionTable)
	{
		UE_LOG(LogTemp, Error, TEXT("No Wave Composition Data Table Found"));
		return;
	}
	
	
	UE_LOG(LogTemp, Warning, TEXT("Composing Wave from Data Table"));

	//Used for Debugging/Find Row Error Context
	static const FString ContextString(TEXT("Wave Composition"));

	/* Our Row object is of type FWaveCompositionRow
	 * THe Current wave # is converted to a string used to search the Name of the Row
	 * 
	 */
	FWaveCompositionRow* WaveCompositionRow = WaveCompositionTable->FindRow<FWaveCompositionRow>(
		*FString::FromInt(CurrentWaveNumber), ContextString, true);
	/*
	 *WaveCompostionRow is a struct that contains the following:
	 *@Params
	* int32 WaveNumber;
	* TArray<FEnemyData> EnemiesToSpawnArray;
	* TSubclassOf<AActor> BossEnemy;
	 */
	if(WaveCompositionRow)
	{
		if(WaveCompositionRow->BossEnemy)
		{
			BossToSpawn = WaveCompositionRow->BossEnemy;
			UE_LOG(LogTemp, Warning, TEXT("Wave Subsystem - ComposeWaveFromDataTable() - Boss Enemy Found"));
		}

		if(WaveCompositionRow->EnemiesToSpawnArray.Num() > 0)
		{
			//For Every Data Asset ...
			for(FEnemyData EnemyData: WaveCompositionRow->EnemiesToSpawnArray)
			{
				//For Every Quantity of Enemy to Spawn
				//Que up the enemies that need to be spawned this Wave
				for(int i = 0 ; i < EnemyData.NumberOfEnemiesToSpawn; i++)
				{
					EnemiesToSpawn.Add(EnemyData.EnemyDataAsset->EnemyClassReference);
					UE_LOG(LogTemp, Warning, TEXT("Wave Subsystem - ComposeWaveFromDataTable() - Enemy Added to SpawnList"));
					TotalEnemiesToSpawn += EnemyData.NumberOfEnemiesToSpawn;
				}
				
			}
		}
	}
}

void UWaveGameInstanceSubsystem::HandleBossSpawn()
{
	
	if(BossToSpawn && !BossSpawned)
	{
		HandleBossDoor(true);
		SpawnEnemy(BossToSpawn, BossSpawnPointLocation);
		BossSpawned = true;
	}
}

void UWaveGameInstanceSubsystem::SpawnPartOfWave()
{
	HandleBossSpawn();
	
	UE_LOG(LogTemp, Warning, TEXT("Wave Subsystem - SpawnPartOfWave() - Spawning Part of Wave"));
	//1-3 enemies can spawn at once - this will help with spreading out the enemies.
	int RandomNumberOfEnemiesToSpawnAtOnce = FMath::RandRange(1, 3);

	for(int i = 0; i < RandomNumberOfEnemiesToSpawnAtOnce; i++)
	{
		if(TotalEnemiesSpawned < EnemiesToSpawn.Num())
		{
			SpawnEnemy(EnemiesToSpawn[TotalEnemiesSpawned], GetRandomStandardSpawnPointLocation());
			TotalEnemiesSpawned++;
		}
		else
		{
			
			GetWorld()->GetTimerManager().ClearTimer(SpawnPartOfWaveTimerHandle);
			bAllEnemiesSpawned = true;
			return;
		}
	}
}

void UWaveGameInstanceSubsystem::SpawnWave()
{
	//Sets a looping timer to spawn random parts of the wave with a delay between each spawn
	//Timer is cleared and finished when all enemies are spawned
	if(!bAllEnemiesSpawned)
	{
		UE_LOG(LogTemp, Warning, TEXT("Wave Subsystem - SpawnWave() - Spawning Wave - Timer Started"));
		GetWorld()->GetTimerManager().SetTimer(SpawnPartOfWaveTimerHandle, this, 
			&UWaveGameInstanceSubsystem::SpawnPartOfWave, TimeBetweenEnemySpawns, true, 1.f);
	}
}

void UWaveGameInstanceSubsystem::EndWave()
{
	//Process of Closing Doors
	CloseLabDoorHandle.Broadcast();
	//TODO::Set Delegate from Boss to Close Lab Doors on Death
	
	//Resetting the wave Data.
	ResetWaveEnemies();
	//Increment Wave
	IncrementWave();
	//Save Game - Game Mode will handle the saving of the game.
	SaveCurrentGameHandle.Broadcast();
	//Start Timer for Next Wave
	GetWorld()->GetTimerManager().SetTimer(TimeToNextWaveHandle, this, &UWaveGameInstanceSubsystem::StartWave, 
	TimeBetweenWaves, false);
}

void UWaveGameInstanceSubsystem::IncrementWave()
{
	CurrentWaveNumber++;

	CurrentWaveHandle.Broadcast(CurrentWaveNumber);
}

void UWaveGameInstanceSubsystem::SpawnEnemy(TSubclassOf<AActor> ActorToSpawn, FVector Location)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* Actor = GetWorld()->SpawnActor<AActor>(ActorToSpawn, Location, FRotator::ZeroRotator, SpawnParameters);
	if(Cast<ATimberEnemyCharacter>(Actor))
	{
		SpawnedEnemies.Add(Cast<ATimberEnemyCharacter>(Actor));
	}
}

void UWaveGameInstanceSubsystem::CheckArrayForEnemy(ATimberEnemyCharacter* Enemy)
{
	//Called when enemy is Destroyed - ATimberEnemyCharacter::TakeDamage()
	if(SpawnedEnemies.Contains(Enemy))
	{
		SpawnedEnemies.Remove(Enemy);
		if(SpawnedEnemies.Num() == 0 && bAllEnemiesSpawned)
		{
			EndWave();
		}
	}
}

void UWaveGameInstanceSubsystem::ResetWaveEnemies()
{
	//Ensures if player kills full array before end of spawning, wave won't end early.
	bAllEnemiesSpawned = false;

	//Reset Wave Data
	TotalEnemiesSpawned = 0;
	TotalEnemiesToSpawn = 0;

	//Boss Reset
	BossToSpawn = nullptr;
	BossSpawned = false;
	
	//Resets the Enemies to Spawn List
	EnemiesToSpawn.Empty();

	//Clears any enemies that are still alive, redundant but safe.
	for(ATimberEnemyCharacter* Enemy : SpawnedEnemies)
	{
		if(Enemy)
		{
			Enemy->Destroy();
		}
	}
	SpawnedEnemies.Empty();
}

void UWaveGameInstanceSubsystem::HandleBossDoor(bool ShouldDoorBeOpen)
{
	if(ShouldDoorBeOpen && BossDoor)
	{
		BossDoor->OpenGarageDoor();
	}
	else
	{
		if(BossDoor)
		{
			BossDoor->CloseGarageDoor();
		}
	}
}
