// Property of Paracosm Industries. Dont use my shit.


#include "BuildSystem/Traps/FrostTrap.h"


// Sets default values
AFrostTrap::AFrostTrap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FrostTrapVentMesh = CreateDefaultSubobject<UStaticMeshComponent>("Frost Trap Vent");
	FrostTrapVentMesh->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AFrostTrap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFrostTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

