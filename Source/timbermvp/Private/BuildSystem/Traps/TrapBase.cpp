// Property of Paracosm Industries. Dont use my shit.


#include "BuildSystem/Traps/TrapBase.h"

#include "BuildSystem/BuildingComponents/TimberBuildingComponentBase.h"
#include "Character/TimberPlayableCharacter.h"
#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Components/BoxComponent.h"


ATrapBase::ATrapBase()
{
	PrimaryActorTick.bCanEverTick = true;

	TrapBaseStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("TrapBaseStaticMesh");
	TrapBaseStaticMesh->SetupAttachment(RootComponent);
	TrapBaseStaticMesh->SetCollisionProfileName(TEXT("DR_BuildableBlockEverything"));

	HitBoxComponent = CreateDefaultSubobject<UBoxComponent>("DamageArea");
	HitBoxComponent->SetupAttachment(TrapBaseStaticMesh);

	//Only Generates Hit Events on Pawns and Building Components
	HitBoxComponent->SetCollisionProfileName(TEXT("DR_HitEventOnly"));
}

void ATrapBase::BeginPlay()
{
	Super::BeginPlay();
	HitBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATrapBase::HitBoxBeginOverlap);
	HitBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ATrapBase::HitBoxEndOverlap);
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

void ATrapBase::FreeUpTrapSlotOnBuildingComponent()
{
	if (ATimberBuildingComponentBase* ParentBuildingComponent = Cast<ATimberBuildingComponentBase>(ParentBuildable))
	{
	
		UE_LOG(LogTemp, Warning, TEXT("Parent Building Component Name: %s"), *ParentBuildable->GetName());
		switch (BuildingComponentTrapDirection)
		{
		case EBuildingComponentTrapDirection::Back :
			ParentBuildingComponent->BackCenterAttachment = nullptr;
			UE_LOG(LogTemp, Warning, TEXT("Trap Base - Parent Building Component Back Trap Set to Nullptr."));
			break;
		case EBuildingComponentTrapDirection::Front :
			ParentBuildingComponent->FrontCenterAttachment = nullptr;
			UE_LOG(LogTemp, Warning, TEXT("Trap Base - Parent Building Component Front Trap Set to Nullptr."));
			break;
		case EBuildingComponentTrapDirection::Default:
			UE_LOG(LogTemp, Warning, TEXT("Trap Base - Trap Direction not set on this Trap. Can not Free up Slot on Parent Building Component."))
			break;
		}

		//We need to also remove the trap from the attachments array when deleting. Important.
		if (ParentBuildingComponent->AttachedBuildingComponents.Contains(this))
		{
			ParentBuildingComponent->AttachedBuildingComponents.Remove(this);
		}
	}
}

void ATrapBase::HandleDeletionOfBuildable()
{
	//Free up the slot on the Parent Building Component
	FreeUpTrapSlotOnBuildingComponent();
	UE_LOG(LogTemp, Warning, TEXT("Freed Up Slot on Parent Building Component."));
	
	//Spawns Loot Value -> Destroy()
	Super::HandleDeletionOfBuildable();
	UE_LOG(LogTemp, Warning, TEXT("Spawned Loot and Destroyed the Actor."));
}

void ATrapBase::HandleDeletionByBuildingComponent()
{
	SpawnLootInRange(BuildableCost.CostOfParts, BuildableCost.CostOfMechanisms, BuildableCost.CostOfUniques);

	Destroy();
}

void ATrapBase::SetCanTrapBeFinalized(bool bCanTrapBeFinalized)
{
	if (bCanTrapBeFinalized != CanTrapBeFinalized)
	{
		CanTrapBeFinalized = bCanTrapBeFinalized;
		OnTrapFinalizationChange.Broadcast(CanTrapBeFinalized);
	}
}

void ATrapBase::HitBoxBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(13, 3.0f, FColor::Red, "Trap HitBox Overlap");
	ATimberEnemyCharacter* Enemy = Cast<ATimberEnemyCharacter>(OtherActor);
	if (Enemy)
	{
		AddEnemyToInsideHitBoxArray(OtherActor);
	}
}

void ATrapBase::HitBoxEndOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ATimberEnemyCharacter* Enemy = Cast<ATimberEnemyCharacter>(OtherActor);
	if (Enemy)
	{
		RemoveEnemyFromInsideHitBoxArray(Enemy);
	}
}

void ATrapBase::AddEnemyToInsideHitBoxArray(AActor* Enemy)
{
	InsideHitBoxArray.Add(Enemy);
}

void ATrapBase::RemoveEnemyFromInsideHitBoxArray(AActor* Enemy)
{
	InsideHitBoxArray.Remove(Enemy);
}
