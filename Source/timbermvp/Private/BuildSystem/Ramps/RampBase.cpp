// Property of Paracosm Industries. Dont use my shit.


#include "BuildSystem/Ramps/RampBase.h"


// Sets default values
ARampBase::ARampBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BuildableType = EBuildableType::Ramp;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMesh;
	VerticalCenterSnap = CreateDefaultSubobject<USceneComponent>("VerticalCenterSnap");
	HorizontalCenterSnap = CreateDefaultSubobject<USceneComponent>("HorizontalCenterSnap");
	VerticalCenterSnap->SetupAttachment(RootComponent);
	HorizontalCenterSnap->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void ARampBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARampBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

