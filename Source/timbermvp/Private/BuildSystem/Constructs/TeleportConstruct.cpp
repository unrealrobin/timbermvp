// Property of Paracosm Industries. Dont use my shit.


#include "BuildSystem/Constructs/TeleportConstruct.h"

#include "Components/BoxComponent.h"


// Sets default values
ATeleportConstruct::ATeleportConstruct()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ComponentRoot =  CreateDefaultSubobject<USceneComponent>("ComponentRoot");
	RootComponent = ComponentRoot;
	TeleportOverlapBox = CreateDefaultSubobject<UBoxComponent>("TeleportOverlapBox");
	TeleportOverlapBox -> SetupAttachment(RootComponent);
	PillarLeft = CreateDefaultSubobject<UStaticMeshComponent>("PillarLeftStaticMesh");
	PillarLeft->SetupAttachment(RootComponent);
	PillarRight = CreateDefaultSubobject<UStaticMeshComponent>("PillarRightStaticMesh");
	PillarRight->SetupAttachment(RootComponent);
	TeleportLandingLocation = CreateDefaultSubobject<USceneComponent>("TeleportLandingLocation");
	TeleportLandingLocation->SetupAttachment(RootComponent);

	TeleportOverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ATeleportConstruct::HandleTeleportOverlap);
}

// Called when the game starts or when spawned
void ATeleportConstruct::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATeleportConstruct::HandleTeleportOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapped the Teleport Construct"));
}

// Called every frame
void ATeleportConstruct::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

