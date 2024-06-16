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

	return SnappedVector;
}

FRotator ATimberBuildSystemManager::SnapToRotation(FRotator CharactersRotation)
{
	//Use Saved Rotation if it is not Zero.
	if(SavedRotation != FRotator::ZeroRotator)
	{
		return SavedRotation;
	}

	/*Examples
	 *
	 * Characters Rotation = { x = 0, y = 0, z = 45 }
	 * Object Rotation = { x = 0, y = 0, z = -135 }
	 * 
	* Characters Rotation = { x = 0, y = 0, z = 90 }
	 * Object Rotation = { x = 0, y = 0, z = -90 }
	 * 
	* Characters Rotation = { x = 0, y = 0, z = 45 }
	 * Object Rotation = { x = 0, y = 0, z = -135 }
	 *
	* Characters Rotation = { x = 0, y = 0, z = -27 }
	 * Object Rotation = { x = 0, y = 0, z =  153}
	 * 
	 */
	
	float CharacterRotationYaw = CharactersRotation.Yaw;

	if(CharacterRotationYaw <= 45 && CharacterRotationYaw >= 0)
	{
		SavedRotation.Yaw = 180;
	}else if(CharacterRotationYaw >= -45 && CharacterRotationYaw < 0)
	{
		SavedRotation.Yaw = 180;
	}else if(CharacterRotationYaw > 45 && CharacterRotationYaw <= 135)
	{
		SavedRotation.Yaw = -90;
	}else if(CharacterRotationYaw < -45 && CharacterRotationYaw > -135)
	{
		SavedRotation.Yaw = 90;
	}else if(CharacterRotationYaw > 135)
	{
		SavedRotation.Yaw = 0;
	}else if(CharacterRotationYaw <= -135)
	{
		SavedRotation.Yaw = 0;
	}

	FString RotationString = SavedRotation.ToString();
	UE_LOG(LogTemp, Warning, TEXT("Rotation: %s"), *RotationString);
	
	return SavedRotation;
}

// Called every frame
void ATimberBuildSystemManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATimberBuildSystemManager::SpawnBuildingComponent(FVector SpawnVector, FRotator SpawnRotator)
{
	const FVector Location = SnapToGrid(SpawnVector);
	const FRotator Rotation = SnapToRotation(SpawnRotator);
	FActorSpawnParameters SpawnParameters;
	FRotator ZeroRotation = FRotator::ZeroRotator;
	//Use the InputTransform as the Location to Spawn the ActiveBuildingComponent
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>
		(ActiveBuildingComponentClass,
			Location,
			Rotation, 
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
void ATimberBuildSystemManager::RotateBuildingComponent()
{
	if(ActiveBuildingComponent)
	{
		SavedRotation.Yaw += 90;
		if (SavedRotation.Yaw >= 360) // Reset Yaw to 0 if it reaches 360
		{
			SavedRotation.Yaw = 0;
		}
		ActiveBuildingComponent->SetActorRotation(SavedRotation);
	}
}

