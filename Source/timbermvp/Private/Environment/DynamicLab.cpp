// Property of Paracosm Industries. Dont use my shit.
#include "Environment/DynamicLab.h"



// Sets default values
ADynamicLab::ADynamicLab()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADynamicLab::BeginPlay()
{
	Super::BeginPlay();
}

void ADynamicLab::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	BuildLab();
}

// Called every frame
void ADynamicLab::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADynamicLab::BuildLab()
{
	EmptyChildComponentArray();
	HandleFloorLayoutChildComponent();
}

void ADynamicLab::HandleFloorLayout()
{
	//Looping through Length -> +X Direction
	for (int i = 0; i <= LabLength -1; i++ )
	{
		for (int k = 0; k <= LabWidth -1; k++ )
		{
			if (EnvironmentFloors)
			{
				int LengthIncrement = i; //(0,1,2,3,4)
				int WidthIncrement = k; //(0,1,2,3,4)
				FVector FloorSpawnLocation = FVector::ZeroVector;
				FloorSpawnLocation.X = LengthIncrement * FloorSizeX;
				FloorSpawnLocation.Y = WidthIncrement * FloorSizeY;

				//Something
			}
		}
	}
}

void ADynamicLab::EmptyChildComponentArray()
{
	if (ChildComponents.Num() > 0)
	{
		for (UChildActorComponent* ChildActor : ChildComponents)
		{
			if (ChildActor)
			{
				ChildActor->DestroyComponent();
			}
		}
		ChildComponents.Empty();
	}
}

void ADynamicLab::HandleFloorLayoutChildComponent()
{
	//Looping through Length -> +X Direction
	for (int i = 0; i <= LabLength -1; i++ )
	{
		//Looping through Width -> +Y Direction
		for (int k = 0; k <= LabWidth -1; k++ )
		{
			//If the EnvironmentFloors Class is set
			if (EnvironmentFloors)
			{
				int LengthIncrement = i; //(0,1,2,3,4)
				int WidthIncrement = k; //(0,1,2,3,4)
				FVector FloorSpawnLocation = FVector::ZeroVector;
				FloorSpawnLocation.X = LengthIncrement * FloorSizeX;
				FloorSpawnLocation.Y = WidthIncrement * FloorSizeY;

				GenerateChildComponent(EnvironmentFloors, FloorSpawnLocation, FRotator::ZeroRotator);
				
			}
		}
	}

	NumberOfFloors = ChildComponents.Num();
}

void ADynamicLab::GenerateChildComponent(TSubclassOf<AActor> BuildableActor, FVector Location, FRotator Rotation)
{
	//Create the New Component
	UChildActorComponent* ChildActorComponent = NewObject<UChildActorComponent>(this,  UChildActorComponent::StaticClass());
	//Attach it to the Root Component of the BP_Lab
	ChildActorComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//Set the Child Actor Class - This Can be the BP of Walls, Floor, Lights, Doors Etc.
	ChildActorComponent->SetChildActorClass(BuildableActor);
	//Set Location
	ChildActorComponent->SetRelativeLocation(Location);
	//Set Rotation
	ChildActorComponent->SetRelativeRotation(Rotation);
	ChildActorComponent->RegisterComponent();

	ChildComponents.Add(ChildActorComponent);
	
}
