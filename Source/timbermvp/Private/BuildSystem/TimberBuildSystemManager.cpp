// Property of Paracosm Industries. Dont use my shit.


#include "BuildSystem/TimberBuildSystemManager.h"
#include "BuildSystem/TimberBuildingComponentBase.h"

// Sets default values
ATimberBuildSystemManager::ATimberBuildSystemManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATimberBuildSystemManager::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector ATimberBuildSystemManager::SnapToGrid(FVector RaycastLocation)
{
	/*Ex Location :
	 *	Initial
	 * { x = 650, y = 20, z = -100 }
	 *
	 * Snapped
	 * { x = 600, y = 0, z = -100 }
	 */

	
	FVector SnappedVector;
	const int SnappedX = (FMath::FloorToInt(RaycastLocation.X) / GridSize ) * GridSize; 
	const int SnappedY = (FMath::FloorToInt(RaycastLocation.Y) / GridSize ) * GridSize;
	const int SnappedZ = (FMath::FloorToInt(RaycastLocation.Z) / GridSize ) * GridSize;
	
	SnappedVector.X = SnappedX;
	SnappedVector.Y = SnappedY;
	SnappedVector.Z = SnappedZ;

	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("Initial Vector: %s"), *RaycastLocation.ToString()));
		GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Red, FString::Printf(TEXT("Snapped Vector: %s"), *SnappedVector.ToString()));
	}

	return SnappedVector;
}

// Called every frame
void ATimberBuildSystemManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATimberBuildSystemManager::SpawnBuildingComponent(FVector SpawnVector, FRotator SpawnRotator)
{
	
	FActorSpawnParameters SpawnParameters;
	FRotator ZeroRotation = FRotator::ZeroRotator;
	//Use the InputTransform as the Location to Spawn the ActiveBuildingComponent
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>
		(ActiveBuildingComponentClass,
			SnapToGrid(SpawnVector),
			ZeroRotation, 
			SpawnParameters);

	ActiveBuildingComponent = Cast<ATimberBuildingComponentBase>(SpawnedActor);
}

void ATimberBuildSystemManager::MoveBuildingComponent(FVector_NetQuantize Location)
{
	if(ActiveBuildingComponent)
	{
		//TODO::Snap To grid logic here.
		ActiveBuildingComponent->SetActorLocation(SnapToGrid(Location));
	}
		
}

