// Property of Paracosm Industries. Dont use my shit.


#include "Loot/EnemyLootDropBase.h"

#include "Character/TimberPlayableCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/Inventory/InventoryManagerComponent.h"
#include "Subsystems/SFX/SFXManagerSubsystem.h"


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

	//Auto Destroy Loot that is not picked up within 60 seconds.
	//If for any reason the loot is spawned outside of the map.
	FTimerHandle NoPickUpTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(NoPickUpTimerHandle,this, &AEnemyLootDropBase::NoPickupAutoDestroy, 360, 
	false );
	
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
		CollisionCapsule->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		CollisionCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
}

void AEnemyLootDropBase::PlaySFX()
{
	USFXManagerSubsystem* SFXManager = GetWorld()->GetGameInstance()->GetSubsystem<USFXManagerSubsystem>();
	if (SFXManager)
	{
		SFXManager->OnLootPickUp.Broadcast();
	}
}

void AEnemyLootDropBase::HandleLootItemOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ATimberPlayableCharacter>(OtherActor))
	{
		PlaySFX();
	}
}

void AEnemyLootDropBase::NoPickupAutoDestroy()
{
	Destroy();
}






