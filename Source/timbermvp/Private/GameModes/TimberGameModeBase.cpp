// Property of Paracosm Industries. Dont use my shit.


#include "GameModes/TimberGameModeBase.h"

#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Environment/TimberEnemySpawnLocations.h"
#include "Kismet/GameplayStatics.h"

void ATimberGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//For Demo Spawn
	//TODO: Create custom Spawn Parameters
	DemoSpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	UWorld* World = GetWorld();
	if(World)
	{
		UGameplayStatics::GetAllActorsOfClass(World, ATimberEnemySpawnLocations::StaticClass(), TimberEnemySpawnPoints);
	}
	GatherAllSpawnLocation(TimberEnemySpawnPoints);
}

//Called from Spawning Object.
void ATimberGameModeBase::SpawnWave(TArray<TSubclassOf<ATimberEnemyCharacter>> EnemiesToSpawn)
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


