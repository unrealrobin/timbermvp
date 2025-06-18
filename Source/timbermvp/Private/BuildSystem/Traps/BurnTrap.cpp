// Property of Paracosm.


#include "BuildSystem/Traps/BurnTrap.h"

#include "Components/BoxComponent.h"
#include "Data/DataAssets/StatusEffects/StatusEffectBase.h"


// Sets default values
ABurnTrap::ABurnTrap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SnapCondition = ESnapCondition::CenterSnap;
	
	BurnTrapInternalsMesh = CreateDefaultSubobject<UStaticMeshComponent>("BurnTrapInternalsMesh");
	BurnTrapInternalsMesh->SetupAttachment(TrapBaseStaticMesh);
	
	HitBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABurnTrap::HandleTrapBeginOverlap);
}

// Called when the game starts or when spawned
void ABurnTrap::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABurnTrap::HandleTrapBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (StatusEffectDataAsset)
	{
		AddEffectToEnemy(OtherActor, StatusEffectDataAsset->StatusEffect);
	}
}

// Called every frame
void ABurnTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

