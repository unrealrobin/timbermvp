// Property of Paracosm Industries. Dont use my shit.


#include "BuildSystem/Traps/TrapBase.h"

#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Components/BoxComponent.h"


ATrapBase::ATrapBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BuildableType = EBuildableType::Trap;
	TrapType = ETrapType::Default;
	
	TrapBaseStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("TrapBaseStaticMesh");
	RootComponent = TrapBaseStaticMesh;
	
	DisableAllStaticMeshCollisions(TrapBaseStaticMesh);

	HitBoxComponent = CreateDefaultSubobject<UBoxComponent>("DamageArea");
	HitBoxComponent->SetupAttachment(RootComponent);
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
		OnTrapFinalizationChange.Broadcast(CanTrapBeFinalized);
	}
}

void ATrapBase::HitBoxBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	ATimberEnemyCharacter* Enemy = Cast<ATimberEnemyCharacter>(OtherActor);
	if(Enemy)
	{
		AddEnemyToInsideHitBoxArray(Enemy);
	}
}

void ATrapBase::HitBoxEndOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	ATimberEnemyCharacter* Enemy = Cast<ATimberEnemyCharacter>(OtherActor);
	if(Enemy)
	{
		RemoveEnemyFromInsideHitBoxArray(Enemy);
	}
	
}

void ATrapBase::AddEnemyToInsideHitBoxArray(ATimberEnemyCharacter* Enemy)
{
	InsideHitBoxArray.Add(Enemy);
}

void ATrapBase::RemoveEnemyFromInsideHitBoxArray(ATimberEnemyCharacter* Enemy)
{
	InsideHitBoxArray.Remove(Enemy);
}

