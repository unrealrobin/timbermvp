// Property of Paracosm Industries. Dont use my shit.


#include "BuildSystem/Traps/TrapBase.h"

#include "Components/BoxComponent.h"


ATrapBase::ATrapBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BuildableType = EBuildableType::Trap;
	TrapType = ETrapType::Default;
	
	TrapBaseStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("TrapBaseStaticMesh");
	RootComponent = TrapBaseStaticMesh;
	
	DisableAllStaticMeshCollisions(TrapBaseStaticMesh);

	DamageAreaBoxComponent = CreateDefaultSubobject<UBoxComponent>("DamageArea");
	DamageAreaBoxComponent->SetupAttachment(RootComponent);
}

void ATrapBase::BeginPlay()
{
	Super::BeginPlay();
	
	OnTrapFinalizationChange.Broadcast(CanTrapBeFinalized);
	
}

void ATrapBase::DisableAllStaticMeshCollisions(UStaticMeshComponent* SomeMesh)
{
	SomeMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	SomeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATrapBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATrapBase::SetCanTrapBeFinalized(bool bCanTrapBeFinalized)
{
	if(bCanTrapBeFinalized != CanTrapBeFinalized)
	{
		CanTrapBeFinalized = bCanTrapBeFinalized;
		GEngine->AddOnScreenDebugMessage(4, 3.0, FColor::Black, "Broadcasting Trap Finalization", false);
		OnTrapFinalizationChange.Broadcast(CanTrapBeFinalized);
	}

}

