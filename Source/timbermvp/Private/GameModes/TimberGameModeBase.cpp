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
	//TODO:: Identify how many of each type of enemy spawn based on wave #
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
	
}

void ATimberGameModeBase::WaveComplete()
{

	//TODO:: What Calls this? We need to track all enemies spawned and when they are all dead, call this function.
	
	CurrentWaveNumber++;
	CurrentWaveNumberHandle.Broadcast(CurrentWaveNumber);

	//Starts a Timer for the next wave to Spawn.
	GetWorld()->GetTimerManager().SetTimer(TimeToNextWaveHandle,this, &ATimberGameModeBase::SpawnDynamicWave, DurationBetweenWaves, 
	false);
	
}




