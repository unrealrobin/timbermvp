// Property of Paracosm Industries. Dont use my shit.


#include "BuildSystem/Constructs/TeleportConstruct.h"

#include "Components/BoxComponent.h"


// Sets default values
ATeleportConstruct::ATeleportConstruct()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TeleportOverlapBox = CreateDefaultSubobject<UBoxComponent>("TeleportOverlapBox");
	RootComponent = TeleportOverlapBox;
	PillarLeft = CreateDefaultSubobject<UStaticMeshComponent>("PillarLeftStaticMesh");
	PillarLeft->SetupAttachment(RootComponent);
	PillarRight = CreateDefaultSubobject<UStaticMeshComponent>("PillarRightStaticMesh");
	PillarRight->SetupAttachment(RootComponent);
	TeleportLandingLocation = CreateDefaultSubobject<USceneComponent>("TeleportLandingLocation");
	TeleportLandingLocation->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATeleportConstruct::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeleportConstruct::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

