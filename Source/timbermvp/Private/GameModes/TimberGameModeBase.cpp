// Property of Paracosm Industries. Dont use my shit.


#include "GameModes/TimberGameModeBase.h"

#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Environment/TimberEnemySpawnLocations.h"
#include "Kismet/GameplayStatics.h"

void ATimberGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if(World)
	{
		UGameplayStatics::GetAllActorsOfClass(World, ATimberEnemySpawnLocations::StaticClass(), TimberEnemySpawnPoints);
	}
		
	GatherAllSpawnLocation(TimberEnemySpawnPoints);
	
}

void ATimberGameModeBase::SpawnWave(TSubclassOf<ATimberEnemyCharacter> TimberEnemy)
{
	FActorSpawnParameters DemoSpawnParameter;
	GetWorld()->SpawnActor<ATimberEnemyCharacter>(TimberEnemy, EnemySpawnPointLocations[0],FRotator::ZeroRotator,
	DemoSpawnParameter);
	
}



void ATimberGameModeBase::GatherAllSpawnLocation(TArray<AActor*> SpawnPoints)
{
	for (AActor* ActorSpawnPoints : SpawnPoints) {
		FVector SpawnPointLocation = ActorSpawnPoints->GetActorLocation();
		EnemySpawnPointLocations.Add(SpawnPointLocation);
	}
}


