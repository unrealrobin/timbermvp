// Property of Paracosm Industries. Dont use my shit.


#include "BuildSystem/BuildingComponents/TimberBuildingComponentBase.h"

#include "BuildSystem/Constructs/ConstructBase.h"
#include "Character/Enemies/TimberEnemyMeleeBase.h"
#include "Components/BoxComponent.h"
#include "Weapons/Projectiles/TimberEnemyProjectile.h"



// Sets default values
ATimberBuildingComponentBase::ATimberBuildingComponentBase()
{
	BuildableType = EBuildableType::BuildingComponent;

	PrimaryActorTick.bCanEverTick = false;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(RootComponent);

	//these settings get overwritten in bp it seems.
	StaticMesh->SetCollisionObjectType(ECC_EngineTraceChannel1); 
	StaticMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	//TODO:: Can we figure out collision with static meshes so that we dont have this fuckin issues. This is unenecessary.
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

void ATimberBuildingComponentBase::HandleDeletionOfBuildable()
{
	
	DeleteAllAttachments();
	
	Super::HandleDeletionOfBuildable();
}

void ATimberBuildingComponentBase::DeleteAllAttachments()
{
	if (AttachedBuildingComponents.Num() > 0)
	{
		for (ABuildableBase* AttachedBuildable : AttachedBuildingComponents)
		{
			//Handle Trap, Handle Construct or other Seperately.
			ATrapBase* AttachedTrap = Cast<ATrapBase>(AttachedBuildable);
			if (AttachedTrap)
			{
				AttachedTrap->HandleDeletionByBuildingComponent();
			}
			AConstructBase* AttachedConstruct = Cast<AConstructBase>(AttachedBuildable);
			if (AttachedConstruct)
			{
				AttachedConstruct->HandleDeletionOfBuildable();
			}
		}
		
		//Empty the Array
		AttachedBuildingComponents.Empty();
	}
}

void ATimberBuildingComponentBase::DestroyAllAttachments()
{
	if (AttachedBuildingComponents.Num() > 0)
	{
		for (ABuildableBase* AttachedBuildingComponent : AttachedBuildingComponents)
		{
			if (AttachedBuildingComponent)
			{
				AttachedBuildingComponent->Destroy();
			}
		}
	}
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
			CheckBuildingComponentOverlapCollisionBox->OnComponentHit.AddDynamic(this, &ATimberBuildingComponentBase::HandleHitBuildingComponent);
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
		DestroyAllAttachments();
		Destroy();
	}
}

void ATimberBuildingComponentBase::CreateSnapPoints()
{
	if (StaticMesh)
	{
		TopSnap = CreateDefaultSubobject<USceneComponent>("TopSnap");
		TopSnap->SetupAttachment(StaticMesh);
		BottomSnap = CreateDefaultSubobject<USceneComponent>("BottomSnap");
		BottomSnap->SetupAttachment(StaticMesh);
		LeftSnap = CreateDefaultSubobject<USceneComponent>("LeftSnap");
		LeftSnap->SetupAttachment(StaticMesh);
		RightSnap = CreateDefaultSubobject<USceneComponent>("RightSnap");
		RightSnap->SetupAttachment(StaticMesh);
		CenterSnap = CreateDefaultSubobject<USceneComponent>("CenterSnap");
		CenterSnap->SetupAttachment(StaticMesh);
		FrontTrapSnap = CreateDefaultSubobject<USceneComponent>("FrontTrapSnap");
		FrontTrapSnap->SetupAttachment(StaticMesh);
		BackTrapSnap = CreateDefaultSubobject<USceneComponent>("BackTrapSnap");
		BackTrapSnap->SetupAttachment(StaticMesh);
	}
}

void ATimberBuildingComponentBase::CreateQuadrantComponents()
{
	if (StaticMesh)
	{
		TopQuadrant = CreateDefaultSubobject<UBoxComponent>("TopQuadrant");
		TopQuadrant->SetupAttachment(StaticMesh);
		BottomQuadrant = CreateDefaultSubobject<UBoxComponent>("BottomQuadrant");
		BottomQuadrant->SetupAttachment(StaticMesh);
		LeftQuadrant = CreateDefaultSubobject<UBoxComponent>("LeftQuadrant");
		LeftQuadrant->SetupAttachment(StaticMesh);
		RightQuadrant = CreateDefaultSubobject<UBoxComponent>("RightQuadrant");
		RightQuadrant->SetupAttachment(StaticMesh);
		CenterQuadrant = CreateDefaultSubobject<UBoxComponent>("CenterQuadrant");
		CenterQuadrant->SetupAttachment(StaticMesh);
	}
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
