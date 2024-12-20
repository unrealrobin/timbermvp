// Property of Paracosm Industries. Dont use my shit.


#include "BuildSystem/Traps/SpikeTrap.h"

#include "Components/BoxComponent.h"


// Sets default values
ASpikeTrap::ASpikeTrap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TrapSpikeMesh  = CreateDefaultSubobject<UStaticMeshComponent>("Spikes");
	TrapSpikeMesh->SetupAttachment(RootComponent);
	DisableAllStaticMeshCollisions(TrapSpikeMesh);

	HitBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASpikeTrap::ExecuteAttack);
}

// Called when the game starts or when spawned
void ASpikeTrap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpikeTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpikeTrap::ExecuteAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* 
OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(2, 4, FColor::Cyan, "Spike TrapTriggerd");
		//TODO:: Play SpikeOut Animation w/ Sound in Anim Notify
		//TODO:: AFter Timer play SpikeIn Animation w/ Sound in Anim Notify
	}
}

