// Property of Paracosm Industries. Dont use my shit.


#include "BuildSystem/TimberBuildSystemManager.h"
#include "BuildSystem/TimberBuildingComponentBase.h"

// Sets default values
ATimberBuildSystemManager::ATimberBuildSystemManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATimberBuildSystemManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATimberBuildSystemManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATimberBuildSystemManager::SpawnBuildingComponent(FVector SpawnVector, FRotator SpawnRotator)
{
	FActorSpawnParameters SpawnParameters;
	FRotator ZeroRotation = FRotator::ZeroRotator;
	//Use the InputTransform as the Location to Spawn the ActiveBuildingComponent
	ActiveBuildingComponent = Cast<ATimberBuildingComponentBase>(GetWorld()->SpawnActor
		(ActiveBuildingComponentClass->StaticClass(),
			&SpawnVector,
			&ZeroRotation, 
			SpawnParameters));
}

