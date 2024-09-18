// Property of Paracosm Industries. Dont use my shit.

#include "BuildSystem/TimberBuildingComponentBase.h"
#include "Components/BuildSystem/BuildSystemManagerComponent.h"

// Sets default values for this component's properties
UBuildSystemManagerComponent::UBuildSystemManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}
// Called when the game starts
void UBuildSystemManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}
// Called every frame
void UBuildSystemManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



/*Spawn Setup*/
FVector UBuildSystemManagerComponent::SnapToGrid(FVector RaycastLocation)
{
	FVector SnappedVector;
	const int SnappedX = (FMath::FloorToInt(RaycastLocation.X) / GridSize ) * GridSize; 
	const int SnappedY = (FMath::FloorToInt(RaycastLocation.Y) / GridSize ) * GridSize;
	
	SnappedVector.X = SnappedX;
	SnappedVector.Y = SnappedY;

	//Not Snapping the Z Axis. This should Spawn the Component Directly on the Ground where they raycast hits.
	SnappedVector.Z = RaycastLocation.Z;

	//Used in the Final Spawn Function on the Controller
	FinalSpawnLocation = SnappedVector;

	return FinalSpawnLocation;
}

FRotator UBuildSystemManagerComponent::SnapToRotation(FRotator CharactersRotation)
{
	//Use last saved Rotation of Component for future components.
	if(SavedRotation != FRotator::ZeroRotator)
	{
		return SavedRotation;
	}

	//TODO:: From where is CharacterRotation passed from and what is it referencing?
	float CharacterRotationYaw = CharactersRotation.Yaw;
	
	// I think the intent here was to always make the Spawned Proxy and the Spawned Component Face the player when spawned.
	//TODO:: Simplify this.
	
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
	
	return SavedRotation;
}

void UBuildSystemManagerComponent::MakeBuildingComponentProxy(ATimberBuildingComponentBase* BuildingComponent)
{
	//Get the Static MeshComponent of the passed in Building Component
	UStaticMeshComponent* MeshComponent = BuildingComponent->FindComponentByClass<UStaticMeshComponent>();

	if(MeshComponent)
	{
		UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(MeshComponent->GetMaterial(0), 
		this);
		
		//Make the Opacity of the Material 0.5f
		if(MaterialInstance)
		{
			//Parameter Created in the Material Instance
			// These are Parameter Nodes that are Created and Defaulted by us in the Material Editor.
			MaterialInstance->SetScalarParameterValue("Opacity", GhostOpacity);
			
			//Can be made Red in the future for unavailable build location.
			MaterialInstance->SetVectorParameterValue("EmmissiveColor", FLinearColor(0.0f, 0.0f, 1.0f, 1.0f));
			
			MeshComponent->SetMaterial(0, MaterialInstance);
		}
	}
}

/* Spawn */
void UBuildSystemManagerComponent::SpawnBuildingComponentProxy(FVector SpawnVector, FRotator SpawnRotator)
{
	if(ActiveBuildingComponentClass)
	{
		const FVector Location = SnapToGrid(SpawnVector);
		const FRotator Rotation = SnapToRotation(SpawnRotator);
		const FActorSpawnParameters SpawnParameters;
		
		//Use the InputTransform as the Location to Spawn the ActiveBuildingComponent
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>
			(ActiveBuildingComponentClass,
				Location,
				Rotation, 
				SpawnParameters);

		ActiveBuildingComponent = Cast<ATimberBuildingComponentBase>(SpawnedActor);
		ActiveBuildingComponent->SetActorEnableCollision(false);

		//Make the Building Component have the "see-through" material look
		MakeBuildingComponentProxy(ActiveBuildingComponent);
	}
}

void UBuildSystemManagerComponent::MoveBuildingComponent(FVector_NetQuantize Location)
{
	if(ActiveBuildingComponent)
	{
		ActiveBuildingComponent->SetActorLocation(SnapToGrid(Location));
	}
}

/*Input Callbacks*/
void UBuildSystemManagerComponent::RotateBuildingComponent()
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

void UBuildSystemManagerComponent::SpawnFinalBuildingComponent(const FVector& Location, const FRotator& Rotation)
{
	FActorSpawnParameters SpawnParameters;

	if(ActiveBuildingComponentClass)
	{
		//Use the InputTransform as the Location to Spawn the ActiveBuildingComponent
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>
			(ActiveBuildingComponentClass,
				Location,
				Rotation, 
				SpawnParameters);

		SpawnedActor->SetActorEnableCollision(true);
	}
}

/*Getters */
ATimberBuildingComponentBase* UBuildSystemManagerComponent::GetActiveBuildingComponent()
{
	return ActiveBuildingComponent;
}



