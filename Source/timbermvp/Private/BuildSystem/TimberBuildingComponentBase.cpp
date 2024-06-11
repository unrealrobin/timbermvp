// Property of Paracosm Industries. Dont use my shit.


#include "BuildSystem/TimberBuildingComponentBase.h"

// Sets default values
ATimberBuildingComponentBase::ATimberBuildingComponentBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATimberBuildingComponentBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATimberBuildingComponentBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

