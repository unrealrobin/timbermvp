// Property of Paracosm Industries. Dont use my shit.


#include "BuildSystem/TimberBuildingComponentBase.h"

#include "Navigation/PathFollowingComponent.h"

// Sets default values
ATimberBuildingComponentBase::ATimberBuildingComponentBase()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMesh;
	StaticMesh->SetCollisionObjectType(ECC_WorldDynamic);
	StaticMesh->SetCollisionResponseToAllChannels(ECR_Block);
	

}

// Called when the game starts or when spawned
void ATimberBuildingComponentBase::BeginPlay()
{
	Super::BeginPlay();
	
}


