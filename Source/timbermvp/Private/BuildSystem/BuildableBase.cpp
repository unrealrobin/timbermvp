// Property of Paracosm Industries. Dont use my shit.


#include "BuildSystem/BuildableBase.h"


// Sets default values
ABuildableBase::ABuildableBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABuildableBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABuildableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
