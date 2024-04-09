// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TimberGameModeBase.generated.h"

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
	virtual void BeginPlay() override;

	void SpawnWave(TArray<TSubclassOf<ATimberEnemyCharacter>> EnemiesToSpawn);

protected:

	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> TimberEnemySpawnPoints;
	
	TArray<FVector> EnemySpawnPointLocations;
	
	void GatherAllSpawnLocation(TArray<AActor*> SpawnPoints);
	
	UPROPERTY(EditAnywhere, Category="Wave Spawn")
	int NumberOfEnemiesToSpawn = 10;

	//Default Spawn Parameters
	FActorSpawnParameters DemoSpawnParameter;
	
};
