﻿// Property of Paracosm Industries. Dont use my shit.


#include "Subsystems/Wave/WaveGameInstanceSubsystem.h"

#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Character/Enemies/TimberEnemyMeleeWeaponBase.h"
#include "Character/Enemies/TimberEnemyRangedBase.h"
#include "Character/Enemies/Boss/BossBase.h"
#include "Data/WaveData.h"
#include "Environment/BossSpawnLocation.h"
#include "Environment/GarageDoorBase.h"
#include "Environment/TimberEnemySpawnLocations.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/SFX/SFXManagerSubsystem.h"
#include "Weapons/TimberWeaponBase.h"
#include "Weapons/TimberWeaponRangedBase.h"


void UWaveGameInstanceSubsystem::SetCurrentWaveNumber(int InWaveNumber)
{
	CurrentWaveNumber = InWaveNumber;
	CurrentWaveHandle.Broadcast(InWaveNumber);
	UE_LOG(LogTemp, Warning, TEXT("Set Wave Number to: %d"), InWaveNumber);
}

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
	//TODO:: Lets find a way to play this for 2 seconds before the rest of the function runs.
	PlayWaveStartSound();
	
	if(CurrentWaveNumber >= 30)
	{
		UE_LOG(LogTemp, Warning, TEXT("Its about to get wild. Endless mode. Bucket of popcorn time. Leaderboards in view."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WaveGameInstanceSubsystem - StartWave() - Starting Wave %d"), CurrentWaveNumber);
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

	//Used for Debugging/Find Row Error Context
	static const FString ContextString(TEXT("Wave Composition"));

	/* Our Row object is of type FWaveCompositionRow
	 * THe Current wave # is converted to a string used to search the Name of the Row
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
			TotalEnemiesToSpawn += 1;
		}
		//Enemies to SpawnArray is the Array in the Data Table of Data Assets containing which enemy to spawn and how many of that enemy to spawn
		//If there is an Array there with atleast 1 entry, we compose the table.
		if(WaveCompositionRow->EnemiesToSpawnArray.Num() > 0)
		{
			//For Every Data Asset ...
			//Ex: Basic Robot -  10
			for(FEnemyData EnemyData: WaveCompositionRow->EnemiesToSpawnArray)
			{
				//For Every Quantity of Enemy to Spawn
				//Que up the enemies that need to be spawned this Wave
				//Looping through the number of enemies to spawn.
				for(int i = 0 ; i < EnemyData.NumberOfEnemiesToSpawn; i++)
				{
					//Adds to the EnemiesToSpawn Array the Class to spawn
					EnemiesToSpawn.Add(EnemyData.EnemyDataAsset->EnemyClassReference);
					//UE_LOG(LogTemp, Warning, TEXT("Wave Subsystem - ComposeWaveFromDataTable() - Enemy Added to SpawnList"));
					TotalEnemiesToSpawn += 1;
				}
				
			}
			UE_LOG(LogTemp, Warning, TEXT("Number of EnemiesToSpawn: %d"), EnemiesToSpawn.Num());
		}
	}
}

void UWaveGameInstanceSubsystem::EarlyStartWave()
{
	//If the Timer is active, but player wants to start the wave early...
	//Ensures we don't Start a wave, then the timer goes off and starts the same wave.
	if (GetWorld()->GetTimerManager().IsTimerActive(TimeToNextWaveHandle))
	{
		//Clears the Timer
		GetWorld()->GetTimerManager().ClearTimer(TimeToNextWaveHandle);
		GetWorld()->GetTimerManager().ClearTimer(UpdatedTimeToNextWaveTimerHandle);

		//Handles Broadcasting Necessary Information
		UpdateTimeToNextWave();
	}
	
	ResetWaveEnemies();
	//Starts the Wave
	StartWave();
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

void UWaveGameInstanceSubsystem::SpawnPartOfWave()
{
	HandleBossSpawn();
	
	//1-3 enemies can spawn at once - this will help with spreading out the enemies.
	int RandomNumberOfEnemiesToSpawnAtOnce = FMath::RandRange(1, 3);
	
	//We need to adjust the Index used based on whether a boss is or isn't spawned. If we dont, then Total Enemies To Spawn will overflow the EnemiesToSpawn Array which
	//doesn't include the boss. So if Enemies to Spawn = 20 and Boss = 1, it can try to find EnemiesToSpawn[21] which would overflow, so we adjust that index whether the boss has been spawned.
	int ArrayIndex;
	if (BossSpawned)
	{
		ArrayIndex = TotalEnemiesSpawned -1;
	}
	else
	{
		ArrayIndex = TotalEnemiesSpawned;
		UE_LOG(LogTemp, Warning, TEXT("Boss not Spawned. ArrayIndex = %d "), ArrayIndex);
	}
	
	for(int i = 0; i < RandomNumberOfEnemiesToSpawnAtOnce; i++)
	{
		/*
		 * TotalEnemiesToSpawn - Decided in Composition, takes Boss into Account.
		 * TotalEnemiesSpawned - Incremented in SpawnEnemy()
		 */
		if(TotalEnemiesSpawned < TotalEnemiesToSpawn)
		{
			SpawnEnemy(EnemiesToSpawn[ArrayIndex], GetRandomStandardSpawnPointLocation());
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(SpawnPartOfWaveTimerHandle);
			//Does not Include is the Boss - this is checking if all basic enemies have been spawned.
			bAllEnemiesSpawned = true;
			//UE_LOG(LogTemp, Warning, TEXT("Wave Subsystem - SpawnPartOfWave() - All Enemies Spawned of Part."));
			return;
		}
	}
}

void UWaveGameInstanceSubsystem::HandleBossSpawn()
{
	if(BossToSpawn && !BossSpawned)
	{
		PlayBossSpawnSound();
		
		//Open the Garage Door.
		OpenBossDoor();
		
		//Spawn the Boss.
		SpawnEnemy(BossToSpawn, BossSpawnPointLocation);
		
	}
}

void UWaveGameInstanceSubsystem::SpawnEnemy(TSubclassOf<AActor> ActorToSpawn, FVector Location)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* Actor = GetWorld()->SpawnActor<AActor>(ActorToSpawn, Location, FRotator::ZeroRotator, SpawnParameters);
	if(Cast<ATimberEnemyCharacter>(Actor))
	{
		SpawnedEnemies.Add(Cast<ATimberEnemyCharacter>(Actor));
		TotalEnemiesSpawned += 1;
		UE_LOG(LogTemp, Warning, TEXT("Total Enemies to Spawn: %d. Total Enemies Spawned: %d"), TotalEnemiesToSpawn, TotalEnemiesSpawned);
	}

	//If we spawn a boss bind to its delegate.
	if (Cast<ABossBase>(Actor))
	{
		//Handles Garage Door Closing on Death
		BindToBossDelegate(Cast<ABossBase>(Actor));
		BossSpawned = true;
	}
}

void UWaveGameInstanceSubsystem::CheckArrayForEnemy(ATimberEnemyCharacter* Enemy)
{
	//Called when enemy is Destroyed - ATimberEnemyCharacter::TakeDamage()
	if(SpawnedEnemies.Contains(Enemy))
	{
		//SpawnedEnemies.Remove(Enemy);
		TotalEnemiesKilled += 1;
		
		UE_LOG(LogTemp, Warning, TEXT("Enemies remaining to Kill: %d"), TotalEnemiesToSpawn - TotalEnemiesKilled);

		//Only counting Basic Enemies
		if(TotalEnemiesKilled == TotalEnemiesToSpawn && bAllEnemiesSpawned)
		{
			EndWave();
		}
	}
}

void UWaveGameInstanceSubsystem::EndWave()
{
	PlayWaveEndSound();

	//Broadcasts the Completed Wave Number
	HandleWaveComplete.Broadcast(CurrentWaveNumber);
	
	//Process of Closing Doors
	CloseLabDoorHandle.Broadcast();
	
	//Resetting the wave Data.
	ResetWaveEnemies();
	
	//Increment Wave - HUD Will always show UPCOMING wave.
	IncrementWave();
	
	//Save Game - Game Mode will handle the saving of the game.
	//SaveCurrentGameHandle.Broadcast();
	
	//Start Timer for Next Wave
	GetWorld()->GetTimerManager().SetTimer(TimeToNextWaveHandle, this, &UWaveGameInstanceSubsystem::StartWave, 
	TimeBetweenWaves, false);
	GetWorld()->GetTimerManager().SetTimer(UpdatedTimeToNextWaveTimerHandle, this, &UWaveGameInstanceSubsystem::UpdateTimeToNextWave, 1.f, true);
}

void UWaveGameInstanceSubsystem::ResetWaveEnemies()
{
	//Ensures if player kills full array before end of spawning, wave won't end early.
	bAllEnemiesSpawned = false;

	//Reset Wave Data
	TotalEnemiesToSpawn = 0;
	TotalEnemiesSpawned = 0;
	TotalEnemiesKilled = 0;

	//Boss Reset
	BossToSpawn = nullptr;
	BossSpawned = false;
	
	//Resets the Enemies to Spawn List
	EnemiesToSpawn.Empty();

	//Clears any enemies that are still alive, redundant but safe.
	if (SpawnedEnemies.Num() > 0)
	{
		for(ATimberEnemyCharacter* Enemy : SpawnedEnemies)
		{
			if(Enemy)
			{
				//Destroys any weapons the enemies carry.
				CheckEnemiesForWeapons(Enemy);
				//Destroys the Enemy
				Enemy->Destroy();
			}
		}
		//Empty's any reference to the enemies in the SpawnedEnemies Array
		SpawnedEnemies.Empty();
	}

}

void UWaveGameInstanceSubsystem::IncrementWave()
{
	CurrentWaveNumber++;

	CurrentWaveHandle.Broadcast(CurrentWaveNumber);
}

void UWaveGameInstanceSubsystem::CheckEnemiesForWeapons(ATimberEnemyCharacter* Enemy)
{
	//These are the only two enemies that carry weapons.
	//They dont share a parent that stores the Equipped Weapon.
	ATimberEnemyMeleeWeaponBase* MeleeWeaponEnemy = Cast<ATimberEnemyMeleeWeaponBase>(Enemy);
	ATimberEnemyRangedBase* RangedWeaponEnemy = Cast<ATimberEnemyRangedBase>(Enemy);
	if (MeleeWeaponEnemy)
	{
		MeleeWeaponEnemy->EquippedWeapon->Destroy();
	}
	if (RangedWeaponEnemy)
	{
		RangedWeaponEnemy->EquippedWeapon->Destroy();
	}
}

void UWaveGameInstanceSubsystem::UpdateTimeToNextWave()
{

	/*
	 * TimeToNextWaveHandle - Timer counting down @param: TimeBetweenWaves
	 * UpdatedTimeToNextWaveTimerHandle - Timer that Broadcast every second the time remaining.
	 */

	//If there is a timer active, get the time remaining.
	if(GetWorld()->GetTimerManager().IsTimerActive(TimeToNextWaveHandle))
	{
		
		TimeToNextWave = FMath::FloorToInt32(GetWorld()->GetTimerManager().GetTimerRemaining(TimeToNextWaveHandle));
		if(TimeToNextWave == 0 )
		{
			//If timer is empty, we don't need to broadcast the time every second.
			GetWorld()->GetTimerManager().ClearTimer(UpdatedTimeToNextWaveTimerHandle);
		}
	}
	else
	{
		TimeToNextWave = 0;
	}

	//Updates the Wave Data on the HUD
	TimeToNextWaveSecondsHandle.Broadcast(TimeToNextWave);
}

void UWaveGameInstanceSubsystem::OpenBossDoor()
{
	if(BossDoor)
	{
		BossDoor->OpenGarageDoor();
	}
}

void UWaveGameInstanceSubsystem::CloseBossDoor()
{
	UE_LOG(LogTemp, Warning, TEXT("Wave Subsystem - CloseBossDoor() - Boss Destroyed - Closing Garage Door"));
	
	if(BossDoor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Garage Door Closes."));
		BossDoor->CloseGarageDoor();
	}
}

void UWaveGameInstanceSubsystem::BindToBossDelegate(ABossBase* BossActor)
{
	//Delegate is Broadcasted on ABossBase when it is destroyed.
	BossActor->OnBossDeath.AddDynamic(this, &UWaveGameInstanceSubsystem::CloseBossDoor);
}

void UWaveGameInstanceSubsystem::PlayWaveStartSound()
{
	USFXManagerSubsystem* SFXManager = GetWorld()->GetGameInstance()->GetSubsystem<USFXManagerSubsystem>();
	if (SFXManager)
	{
		SFXManager->OnWaveStart.Broadcast();
	}
}

void UWaveGameInstanceSubsystem::PlayWaveEndSound()
{
	USFXManagerSubsystem* SFXManager = GetWorld()->GetGameInstance()->GetSubsystem<USFXManagerSubsystem>();
	if (SFXManager)
	{
		SFXManager->OnWaveEnd.Broadcast();
	}
}

void UWaveGameInstanceSubsystem::PlayBossSpawnSound()
{
	USFXManagerSubsystem* SFXManager = GetWorld()->GetGameInstance()->GetSubsystem<USFXManagerSubsystem>();
	if (SFXManager)
	{
		SFXManager->OnBossSpawn.Broadcast();
	}
}

void UWaveGameInstanceSubsystem::FullStop()
{
	//This acts as a full stop for all Wave Spawning things.
	
	//This ends the wave and handles some other cleanup but also starts new timers for next wave.
	EndWave();
	//Stops all ongoing timers for spawning enemies and incrementing waves.
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

