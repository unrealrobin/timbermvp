// Property of Paracosm Industries. Dont use my shit.


#include "BuildSystem/TimberBuildSystemManager.h"

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

