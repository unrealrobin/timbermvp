// Property of Paracosm Industries. Dont use my shit.


#include "GameModes/TimberGameModeBase.h"

#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Environment/TimberEnemySpawnLocations.h"
#include "Kismet/GameplayStatics.h"

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
}

void ATimberGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Gets the Timer and Converts the time to an Int. To be used for UI purposes to inform player of time to next wave.
	if(GetWorld()->GetTimerManager().IsTimerActive(TimeToNextWaveHandle))
	{
		TimeToNextWave = FMath::CeilToInt(GetWorld()->GetTimerManager().GetTimerRemaining(TimeToNextWaveHandle));
	}
}

//Called from Spawning Object.
/*void ATimberGameModeBase::SpawnWave(TArray<TSubclassOf<ATimberEnemyCharacter>> EnemiesToSpawn)
{
	for (int i = 0;  i <= NumberOfEnemiesToSpawn; i++)
	{
		//Random Spawn Location Index
		float RandomLocation = FMath::RandRange(0, EnemySpawnPointLocations.Num() - 1);
		//Random Enemy To Spawn
		float RandomEnemy = FMath::RandRange(0, EnemiesToSpawn.Num() - 1);
		GetWorld()->SpawnActor<ATimberEnemyCharacter>(EnemiesToSpawn[RandomEnemy], 
		EnemySpawnPointLocations[RandomLocation],
		FRotator::ZeroRotator, DemoSpawnParameter);
	}
}*/

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
	//TODO:: Identify how many of each type of enemy spawn based on wave #
	Wave.GhoulCount = FMath::RandRange(1, (CurrentWaveNumber + 4));
	
	Wave.GoblinCount = FMath::RandRange(0, CurrentWaveNumber + 2);
}

void ATimberGameModeBase::SpawnEnemyAtLocation(TSubclassOf<ATimberEnemyCharacter> EnemyClassName)
{
	float RandomLocation = FMath::RandRange(0, EnemySpawnPointLocations.Num() - 1);
	GetWorld()->SpawnActor<ATimberEnemyCharacter>(EnemyClassName, 
	                                              EnemySpawnPointLocations[RandomLocation],
	                                              FRotator::ZeroRotator, DemoSpawnParameter);
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

void ATimberGameModeBase::WaveComplete()
{

	//TODO:: What Calls this? We need to track all enemies spawned and when they are all dead, call this function.
	
	CurrentWaveNumber++;
	CurrentWaveNumberHandle.Broadcast(CurrentWaveNumber);

	//Starts a Timer for the next wave to Spawn.
	GetWorld()->GetTimerManager().SetTimer(TimeToNextWaveHandle,this, &ATimberGameModeBase::SpawnDynamicWave, 60.0f, 
	false);
	
}


