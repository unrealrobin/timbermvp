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
	HandleWallLayoutChildComponent();
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
	NumberOfFloors = 0;
	NumberOfCeilingTiles = 0;
	//Looping through Length -> +X Direction
	for (int i = 0; i <= LabLength -1; i++ )
	{
		//Looping through Width -> +Y Direction
		for (int k = 0; k <= LabWidth -1; k++ )
		{
			//Handling Lab Floor Tile Generation
			if (EnvironmentFloors)
			{
				int LengthIncrement = i; //(0,1,2,3,4)
				int WidthIncrement = k; //(0,1,2,3,4)
				FVector FloorSpawnLocation = FVector::ZeroVector;
				FloorSpawnLocation.X = LengthIncrement * FloorSizeX;
				FloorSpawnLocation.Y = WidthIncrement * FloorSizeY;

				GenerateChildComponent(EnvironmentFloors, FloorSpawnLocation, FRotator::ZeroRotator);
				NumberOfFloors++;
			}

			//Handling Lab Ceiling Tile Generation
			if (EnvironmentFloors)
			{
				int LengthIncrement = i; //(0,1,2,3,4)
				int WidthIncrement = k; //(0,1,2,3,4)
				FVector CeilingSpawnLocation = FVector::ZeroVector;
				CeilingSpawnLocation.X = LengthIncrement * FloorSizeX;
				CeilingSpawnLocation.Y = WidthIncrement * FloorSizeY;

				float CeilingHeight = WallSizeZ * LabLevels;
				CeilingSpawnLocation.Z = CeilingHeight;

				GenerateChildComponent(EnvironmentFloors, CeilingSpawnLocation, FRotator::ZeroRotator);
				NumberOfCeilingTiles++;
			}
		}
	}
}

void ADynamicLab::HandleWallLayoutChildComponent()
{
	NumberOfWalls = 0;
	//How many Levels of Walls.
	for (int i = 0; i <= LabLevels - 1; i++ )
	{
		int LevelIncrement = i;
		FVector InitialWallLocationLength = FVector::ZeroVector;
		InitialWallLocationLength.Y -= 205.0f;
		InitialWallLocationLength.Z += (LevelIncrement) * WallSizeZ;

		//How many "Runs" of the walls -> +X Direction
		for (int k = 0; k <= LabLength - 1; k++ )
		{
			if (EnvironmentWalls)
			{
				//Using the Increment to calculate the Location of the wall.
				FVector UpdatedWallLocation = InitialWallLocationLength;
				UpdatedWallLocation.X += k * FloorSizeX;
				GenerateChildComponent(EnvironmentWalls, UpdatedWallLocation, FRotator::ZeroRotator);
				NumberOfWalls++;
				
				FVector AdjacentWallLocation = UpdatedWallLocation;
				AdjacentWallLocation.Y += FloorSizeX * LabWidth + 15;
				GenerateChildComponent(EnvironmentWalls, AdjacentWallLocation, FRotator::ZeroRotator);
				
				NumberOfWalls++;
			}
		}

		FVector InitialWallLocationWidth = FVector::ZeroVector;
		InitialWallLocationWidth.X -= 205.0f;
		InitialWallLocationWidth.Z += (LevelIncrement) * WallSizeZ;
		FRotator AdjustedWallRotation = FRotator::ZeroRotator;
		AdjustedWallRotation.Yaw += 90.0f;

		
		//How many "Widths" of the walls -> Y Directions
		for (int j = 0; j <= LabWidth - 1; j++ )
		{
			if (EnvironmentWalls)
			{
				//Using the Increment to calculate the Location of the wall.
				FVector UpdatedWallLocation = InitialWallLocationWidth;
				UpdatedWallLocation.Y += j * FloorSizeX;
				GenerateChildComponent(EnvironmentWalls, UpdatedWallLocation, AdjustedWallRotation);

				NumberOfWalls++;

				FVector AdjacentWallLocation = UpdatedWallLocation;
				AdjacentWallLocation.X += FloorSizeY * LabLength + 15;
				GenerateChildComponent(EnvironmentWalls, AdjacentWallLocation, AdjustedWallRotation);
				
				NumberOfWalls++;
			}
		}
	}
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
