// Property of Paracosm Industries. Dont use my shit.


#include "BuildSystem/BuildingComponents/TimberBuildingComponentBase.h"

#include "Character/Enemies/TimberEnemyMeleeBase.h"
#include "Components/BoxComponent.h"
#include "Weapons/Projectiles/TimberEnemyProjectile.h"



// Sets default values
ATimberBuildingComponentBase::ATimberBuildingComponentBase()
{
	BuildableType = EBuildableType::BuildingComponent;

	PrimaryActorTick.bCanEverTick = false;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMesh;

	//these settings get overwritten in bp it seems.
	StaticMesh->SetCollisionObjectType(ECC_EngineTraceChannel1); 
	StaticMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	NavCollisionBox = CreateDefaultSubobject<UBoxComponent>("NavCollisionBox");
	NavCollisionBox->SetupAttachment(StaticMesh);
	NavCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	NavCollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	NavCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	/* Snap Points for Building Component to Building Component Snapping.*/
	CreateSnapPoints();

	/*Quadrants for Raycast Collision to know where to snap too.*/
	CreateQuadrantComponents();
}




// Called when the game starts or when spawned
void ATimberBuildingComponentBase::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("Begin Play on Proxy Building Component."));
	SetupProxyCollisionHandling();
}

void ATimberBuildingComponentBase::SetupProxyCollisionHandling()
{
	if (bIsProxy == true)
	{
		//Will check if overlapping another Buildings Components Overlap Box, if it does, Do Not Allow Finalization.
		//Is only necessary if Proxy
		//Using New Object because Runtime Creation.
		UBoxComponent* CheckBuildingComponentOverlapCollisionBox = NewObject<UBoxComponent>(this, "CheckBuildingComponentOverlapCollisionBox");
		if (CheckBuildingComponentOverlapCollisionBox)
		{
			CheckBuildingComponentOverlapCollisionBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			CheckBuildingComponentOverlapCollisionBox->RegisterComponent();
			
			/*CheckBuildingComponentOverlapCollisionBox->OnComponentBeginOverlap.AddDynamic(this, 
				&ATimberBuildingComponentBase::HandleOverlappedBuildingComponent);*/
			CheckBuildingComponentOverlapCollisionBox->OnComponentHit.AddDynamic(this, 
				&ATimberBuildingComponentBase::HandleHitBuildingComponent);
			/*CheckBuildingComponentOverlapCollisionBox->OnComponentEndOverlap.AddDynamic(this, 
				&ATimberBuildingComponentBase::HandleEndOverlappedBuildingComponent);*/

			CheckBuildingComponentOverlapCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			CheckBuildingComponentOverlapCollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
			//Checks if this Box Component is Overlapping with another Building Component (Walls/Floor).
			//Setting the Object type to Building Component object type.
			CheckBuildingComponentOverlapCollisionBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);

			//Makes it visible in the Editor.
			AddInstanceComponent(CheckBuildingComponentOverlapCollisionBox);
			UE_LOG(LogTemp, Warning, TEXT("Check Building Component Overlap Collision Box Created."));
		}
		
	}
}

void ATimberBuildingComponentBase::BuildingComponentTakeDamage(float AmountOfDamage, AActor* DamagingActor)
{
	//Who hit what for how much?
	UE_LOG(LogTemp, Warning, TEXT("%lS took %f damage from %s."), *GetName(), AmountOfDamage, *DamagingActor->GetName());
	
	ComponentDurability = ComponentDurability - AmountOfDamage;

	if (ComponentDurability <= 0)
	{
		Destroy();
	}
}

void ATimberBuildingComponentBase::CreateSnapPoints()
{
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
	FrontTrapSnap = CreateDefaultSubobject<USceneComponent>("FrontTrapSnap");
	FrontTrapSnap->SetupAttachment(RootComponent);
	BackTrapSnap = CreateDefaultSubobject<USceneComponent>("BackTrapSnap");
	BackTrapSnap->SetupAttachment(RootComponent);
}

void ATimberBuildingComponentBase::CreateQuadrantComponents()
{
	TopQuadrant = CreateDefaultSubobject<UBoxComponent>("TopQuadrant");
	TopQuadrant->SetupAttachment(RootComponent);
	BottomQuadrant = CreateDefaultSubobject<UBoxComponent>("BottomQuadrant");
	BottomQuadrant->SetupAttachment(RootComponent);
	LeftQuadrant = CreateDefaultSubobject<UBoxComponent>("LeftQuadrant");
	LeftQuadrant->SetupAttachment(RootComponent);
	RightQuadrant = CreateDefaultSubobject<UBoxComponent>("RightQuadrant");
	RightQuadrant->SetupAttachment(RootComponent);
	CenterQuadrant = CreateDefaultSubobject<UBoxComponent>("CenterQuadrant");
	CenterQuadrant->SetupAttachment(RootComponent);
}

void ATimberBuildingComponentBase::HandleOverlappedBuildingComponent(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	ATimberBuildingComponentBase* OtherOverlappedBuildingComponent = Cast<ATimberBuildingComponentBase>(OtherActor);
	if (OtherOverlappedBuildingComponent && OtherOverlappedBuildingComponent != this)
	{
		//This will let us know the Other Actor that we are overlapping
		UE_LOG(LogTemp, Warning, TEXT("Proxy: %s Overlap Check Box Overlapping with Building Component Actor: %s."), *GetClass()->GetName(), *OtherOverlappedBuildingComponent->GetName());
		
		if (OtherOverlappedBuildingComponent)
		{
			//Make sure that the Final Spawning of the Building Component Cant Be Finalized.
			bCanBuildableBeFinalized = false;
			UE_LOG(LogTemp, Warning, TEXT("Proxy is overlapping another Building Component. Cant be Finalized."));
		}
	}
}

void ATimberBuildingComponentBase::HandleHitBuildingComponent(
	UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	const FHitResult& Hit)
{
	ATimberBuildingComponentBase* OtherOverlappedBuildingComponent = Cast<ATimberBuildingComponentBase>(OtherActor);
	if (OtherOverlappedBuildingComponent && OtherOverlappedBuildingComponent != this)
	{
		//This will let us know the Other Actor that we are overlapping
		UE_LOG(LogTemp, Warning, TEXT("Proxy: %s Overlap Check Box Overlapping with Building Component Actor: %s."), *GetClass()->GetName(), *OtherOverlappedBuildingComponent->GetName());
		
		if (OtherOverlappedBuildingComponent)
		{
			//Make sure that the Final Spawning of the Building Component Cant Be Finalized.
			bCanBuildableBeFinalized = false;
			UE_LOG(LogTemp, Warning, TEXT("Proxy is overlapping another Building Component. Cant be Finalized."));
		}
	}
}

void ATimberBuildingComponentBase::HandleEndOverlappedBuildingComponent(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ATimberBuildingComponentBase* OverlappedBuildingComponent = Cast<ATimberBuildingComponentBase>(OtherActor);
	if (OverlappedBuildingComponent)
	{
		//Make sure that the Final Spawning of the Building Component Cant Be Finalized.
		bCanBuildableBeFinalized = true;
		UE_LOG(LogTemp, Warning, TEXT("Proxy is no longer overlapping another Building Component. Can be Finalized."));
	}
}
