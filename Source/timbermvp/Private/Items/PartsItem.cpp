// Property of Paracosm Industries. Dont use my shit.


#include "Items/PartsItem.h"

#include "Character/TimberPlayableCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/Inventory/InventoryManagerComponent.h"


// Sets default values
APartsItem::APartsItem()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(RootComponent);
	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>("CollisionCapsule");
	CollisionCapsule->SetupAttachment(StaticMesh);

	CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &APartsItem::HandlePartsOverlap);
}

// Called when the game starts or when spawned
void APartsItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void APartsItem::HandlePartsOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	//When overlapped, add coins to the players state inventory
	if(OtherActor == Cast<ATimberPlayableCharacter>(OtherActor))
	{
		// Add parts to player Inventory
		Cast<ATimberPlayableCharacter>(OtherActor)->InventoryManager->AddPartsToInventory(1);

		// Destroy the parts item
		Destroy();
	}
}





