// Property of Paracosm Industries. Dont use my shit.


#include "BuildSystem/TimberBuildSystemManager.h"
#include "BuildSystem/TimberBuildingComponentBase.h"
#include "Materials/MaterialInstanceConstant.h"

// Sets default values
ATimberBuildSystemManager::ATimberBuildSystemManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATimberBuildSystemManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATimberBuildSystemManager::MakeBuildingComponentGhost(ATimberBuildingComponentBase* BuildingComponent)
{
	//Get the Static MeshComponent of the BP Item
	UStaticMeshComponent* MeshComponent = BuildingComponent->FindComponentByClass<UStaticMeshComponent>();

	if(MeshComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Mesh Component Found."));
		UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(MeshComponent->GetMaterial(0), this);
		//Make the Material a Dynamic Material
		
		//Make the Opacity of the Material 0.5f
		if(MaterialInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("Material Instance Found."));

			//Parameter Created in the Material Instance
			MaterialInstance->SetScalarParameterValue("Opacity", GhostOpacity);
			MaterialInstance->SetVectorParameterValue("EmmissiveColor", FLinearColor(0.0f, 0.0f, 1.0f, 1.0f));
			MeshComponent->SetMaterial(0, MaterialInstance);
		}
	}
}

void ATimberBuildSystemManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ATimberBuildSystemManager::SnapToGrid(FVector RaycastLocation)
{
	/*Ex Location :
	 *	Initial
	 * { x = 650.56, y = 20.1, z = -100.3 }
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

	FinalSpawnLocation = SnappedVector;

	return SnappedVector;
}

FRotator ATimberBuildSystemManager::SnapToRotation(FRotator CharactersRotation)
{
	//Use Saved Rotation if it is not Zero.
	if(SavedRotation != FRotator::ZeroRotator)
	{
		return SavedRotation;
	}
	
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

	FinalSpawnRotation = SavedRotation;
	/*FString RotationString = SavedRotation.ToString();
	UE_LOG(LogTemp, Warning, TEXT("Rotation: %s"), *RotationString);*/
	
	return SavedRotation;
}

void ATimberBuildSystemManager::SpawnBuildingComponent(FVector SpawnVector, FRotator SpawnRotator)
{
	const FVector Location = SnapToGrid(SpawnVector);
	const FRotator Rotation = SnapToRotation(SpawnRotator);
	FActorSpawnParameters SpawnParameters;
	//Use the InputTransform as the Location to Spawn the ActiveBuildingComponent
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>
		(ActiveBuildingComponentClass,
			Location,
			Rotation, 
			SpawnParameters);

	ActiveBuildingComponent = Cast<ATimberBuildingComponentBase>(SpawnedActor);

	//Make the Building Piece have see through material.
	MakeBuildingComponentGhost(ActiveBuildingComponent);
	
}

void ATimberBuildSystemManager::MoveBuildingComponent(FVector_NetQuantize Location)
{
	if(ActiveBuildingComponent)
	{
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
		//Updating FinalSpawnRotation after Player Rotates Component
		FinalSpawnRotation = SavedRotation;
	}
}

void ATimberBuildSystemManager::SpawnFinalBuildingComponent(const FVector& Location, const FRotator& Rotation)
{
	FActorSpawnParameters SpawnParameters;
	
	//Use the InputTransform as the Location to Spawn the ActiveBuildingComponent
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>
		(ActiveBuildingComponentClass,
			Location,
			Rotation, 
			SpawnParameters);
}

