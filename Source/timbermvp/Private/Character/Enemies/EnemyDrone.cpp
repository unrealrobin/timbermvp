// Property of Paracosm.


#include "Character/Enemies/EnemyDrone.h"


// Sets default values
AEnemyDrone::AEnemyDrone()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyDrone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyDrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

