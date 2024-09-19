// Property of Paracosm Industries. Dont use my shit.


#include "BuildSystem/TimberBuildingComponentBase.h"

#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "Navigation/PathFollowingComponent.h"

// Sets default values
ATimberBuildingComponentBase::ATimberBuildingComponentBase()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMesh;
	StaticMesh->SetCollisionObjectType(ECC_EngineTraceChannel1);
	StaticMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &ATimberBuildingComponentBase::HandleOverlapNotifies);

	/* Snap Points for Building Component to Building Component Snapping.*/
	TopSnap = CreateDefaultSubobject<USceneComponent>("TopSnap");
	TopSnap->SetupAttachment(RootComponent);
	BottomSnap = CreateDefaultSubobject<USceneComponent>("BottomSnap");
	BottomSnap->SetupAttachment(RootComponent);
	LeftSnap = CreateDefaultSubobject<USceneComponent>("LeftSnap");
	LeftSnap->SetupAttachment(RootComponent);
	RightSnap = CreateDefaultSubobject<USceneComponent>("RightSnap");
	RightSnap->SetupAttachment(RootComponent);
	CenterSnap = CreateDefaultSubobject<USceneComponent>("CenterSnap");
	CenterSnap->SetupAttachment(RootComponent);

	/*Quadrants for Raycast Collision to know where to snap too.*/
	TopQuadrant = CreateDefaultSubobject<UBoxComponent>("TopQuadrantLeft");
	TopQuadrant->SetupAttachment(RootComponent);
	BottomQuadrant = CreateDefaultSubobject<UBoxComponent>("BottomQuadrantBottom");
	BottomQuadrant->SetupAttachment(RootComponent);
	LeftQuadrant = CreateDefaultSubobject<UBoxComponent>("LeftQuadrantCenter");
	LeftQuadrant->SetupAttachment(RootComponent);
	RightQuadrant = CreateDefaultSubobject<UBoxComponent>("RightQuadrant");
	RightQuadrant->SetupAttachment(RootComponent);
	CenterQuadrant = CreateDefaultSubobject<UBoxComponent>("CenterQuadrant");
	CenterQuadrant->SetupAttachment(RootComponent);
	
}

void ATimberBuildingComponentBase::BuildingComponentTakeDamage(float AmountOfDamage)
{
	ComponentDurability = ComponentDurability - AmountOfDamage;

	UE_LOG(LogTemp, Warning, TEXT("Building Component Durability: %f."), ComponentDurability);

	if(ComponentDurability <= 0 )
	{
		//TODO::Add some destroyed animation
		PlayDestroyedAnimation();
		Destroy();
	}
}

void ATimberBuildingComponentBase::PlayDestroyedAnimation()
{
	UE_LOG(LogTemp, Warning, TEXT("Building Component Destroyed."));
}

// Called when the game starts or when spawned
void ATimberBuildingComponentBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATimberBuildingComponentBase::HandleOverlapNotifies(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	//Will handle attacks from the Enemy.
	if(OtherActor->IsA(ATimberEnemyCharacter::StaticClass()))
	{
		BuildingComponentTakeDamage(25.f);
	}
}


