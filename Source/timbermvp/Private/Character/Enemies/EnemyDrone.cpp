// Property of Paracosm.


#include "Character/Enemies/EnemyDrone.h"

#include "Components/SplineComponent.h"
#include "Environment/EnemyDroneSplinePath.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEnemyDrone::AEnemyDrone()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyDrone::BeginPlay()
{
	Super::BeginPlay();

	// Find all Actors of Type EnemyDroneSplinePaths
	GetAllDroneSplinePathActors();
	// From a list of spline paths, select one at random
		// Set as a reference on this drone.
	SelectRandomSplinePath();
}

void AEnemyDrone::SelectRandomSplinePath()
{
	if (SplinePathActors.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, SplinePathActors.Num() - 1);
		SplinePathRef = Cast<AEnemyDroneSplinePath>(SplinePathActors[RandomIndex]);
		if (SplinePathRef)
		{
			SplineComponent = SplinePathRef->SplineComponent;
			if (SplineComponent)
			{
				
				bIsApproachSpline = true;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No spline paths found!"));
	}
}

void AEnemyDrone::GetAllDroneSplinePathActors()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyDroneSplinePath::StaticClass(), SplinePathActors);
	if (SplinePathActors.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No spline path actors found in the level!"));
	}
}

void AEnemyDrone::MoveAlongSplinePath(float DeltaTime)
{
	if (SplineComponent)
	{
		//Getting the length of the spline
		const float SplineLength = SplineComponent->GetSplineLength();
		//Setting the Would be Distance along the spline scaled with Delta Time (1 frame)
		CurrentDistanceAlongSplinePath += DroneFlightSpeed * DeltaTime;

		if (CurrentDistanceAlongSplinePath > SplineLength)
		{
			//Resetting the current distance to 0 (completed a full loop)
			CurrentDistanceAlongSplinePath = 0.0f;
		}

		//Using the distance we are along the spline to get the new Move Target.
		FVector NewMoveTarget = SplineComponent->GetLocationAtDistanceAlongSpline(CurrentDistanceAlongSplinePath, ESplineCoordinateSpace::World);
		FRotator NewRotation = SplineComponent->GetRotationAtDistanceAlongSpline(CurrentDistanceAlongSplinePath, ESplineCoordinateSpace::World);

		SetActorLocationAndRotation(NewMoveTarget, NewRotation);
	}
}

void AEnemyDrone::FlyToSplineStart(float DeltaTime)
{
	//Initialized to 0 (Can be any distance along the spline)
	FVector SplineStartLocation  = SplineComponent->GetLocationAtDistanceAlongSpline(CurrentDistanceAlongSplinePath, ESplineCoordinateSpace::World);
	FVector DroneLocation = GetActorLocation();
	//Getting a vector that points from drone to spline start location
	FVector MoveDirection = (SplineStartLocation - DroneLocation).GetSafeNormal();
	//Getting a point scaled by the speed of the drone along the Move Direction
	FVector NewLocation = DroneLocation + MoveDirection * DroneApproachSpeed * DeltaTime;

	SetActorLocation(NewLocation);
	SetActorRotation(MoveDirection.Rotation());

	//Checking if within range of the spline start location
	if (FVector::Dist(NewLocation, SplineStartLocation) < 10.0f)
	{
		//When within range of spline start, move to standard spline flight trajectory.
		bIsApproachSpline = false;
		bShouldMoveAlongSpline = true;
	}
}

// Called every frame
void AEnemyDrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (SplineComponent)
	{
		if (bIsApproachSpline)
		{
			FlyToSplineStart(DeltaTime);
		}
		else if (bShouldMoveAlongSpline)
		{
			MoveAlongSplinePath(DeltaTime);
		}
	}
}

