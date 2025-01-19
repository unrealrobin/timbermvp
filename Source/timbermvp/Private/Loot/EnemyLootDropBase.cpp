// Property of Paracosm Industries. Dont use my shit.


#include "Loot/EnemyLootDropBase.h"

#include "Character/TimberPlayableCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/Inventory/InventoryManagerComponent.h"


// Sets default values
AEnemyLootDropBase::AEnemyLootDropBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>("CollisionCapsule");
	RootComponent = CollisionCapsule;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(RootComponent);


	SetCollisionProperties();

	
}

// Called when the game starts or when spawned
void AEnemyLootDropBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyLootDropBase::SetCollisionProperties()
{
	if(StaticMesh)
	{
		StaticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

	if(CollisionCapsule)
	{
		CollisionCapsule->SetCollisionObjectType(ECC_WorldDynamic);
		CollisionCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
		CollisionCapsule->SetCollisionResponseToChannel(ECC_EngineTraceChannel1, ECR_Block);
		CollisionCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
}






