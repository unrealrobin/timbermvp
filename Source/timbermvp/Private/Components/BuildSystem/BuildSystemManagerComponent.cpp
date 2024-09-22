// Property of Paracosm Industries. Dont use my shit.


#include "Components/BuildSystem/BuildSystemManagerComponent.h"
#include "BuildSystem/TimberBuildingComponentBase.h"

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

/*Component to Component Snapping*/
void UBuildSystemManagerComponent::HandleBuildingComponentSnapping(FHitResult HitQuadrant, FHitResult HitActor)
{
	EBuildingComponentOrientation ProxyBuildingComponentOrientation = ActiveBuildingComponent->BuildingComponentOrientation;
	
	// Get HitResult Building Component Enum Orientation
	const ATimberBuildingComponentBase* BuildingComponent = Cast<ATimberBuildingComponentBase>(HitActor.GetActor());
	EBuildingComponentOrientation PlacedBuildingComponentOrientation = BuildingComponent->BuildingComponentOrientation;
	
	const FString HitQuadrantName = *HitQuadrant.GetComponent()->GetName();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *HitQuadrantName);

	// Returns the condition to snap based on Orientation
	int SnappingConditionNum = SnappingCondition(ProxyBuildingComponentOrientation, PlacedBuildingComponentOrientation);

	//Handles the Movement
	switch (SnappingConditionNum)
	{
	case 1: VerticalSnapCondition(HitActor, HitQuadrant);
		break;
	case 2: HorizontalSnapCondition(HitActor, HitQuadrant);
		break;
	case 3: VerticalToHorizontalSnapCondition(HitActor, HitQuadrant);
		break;
	case 4: HorizontalToVerticalSnapCondition(HitActor, HitQuadrant);
		break;
	case 5: return;
	default: return;
	}
}

int UBuildSystemManagerComponent::SnappingCondition(
	EBuildingComponentOrientation Orientation1, EBuildingComponentOrientation Orientation2)
{
	if(Orientation1 == Orientation2 && Orientation1 == EBuildingComponentOrientation::Vertical)
	{
		//Both Vertical
		return 1;
	}

	if(Orientation1 == Orientation2 && Orientation1 == EBuildingComponentOrientation::Horizontal)
	{
		//Both Horizontal
		return 2;
	}
	
	if(Orientation1 != Orientation2 && Orientation1 == EBuildingComponentOrientation::Vertical)
	{
		//Orientation 1 = Vertical, Orientation 2 = Horizontal
		return 3;
	}

	if(Orientation1 != Orientation2 && Orientation1 == EBuildingComponentOrientation::Horizontal)
	{
		//Orientation 1 = Horizontal, Orientation 2 = Vertical
		return 3;
	}

	return 4;	
}

int UBuildSystemManagerComponent::QuadrantCondition(FString QuadrantName)
{
	if(QuadrantName == FString("TopQuadrant"))
	{
		return 1;
	}
	if(QuadrantName == FString("RightQuadrant"))
	{
		return 2;
	}
	if(QuadrantName == FString("BottomQuadrant"))
	{
		return 3;
	}
	if(QuadrantName == FString("LeftQuadrant"))
	{
		return 4;
	}
	if(QuadrantName == FString("CenterQuadrant"))
	{
		return 5;
	}

	return 6;
}

void UBuildSystemManagerComponent::VerticalSnapCondition(FHitResult HitActor, FHitResult HitQuadrant)
{
	FVector ProxySnapLocation;
	FVector HitActorSnapLocation;

	int QuadrantConditionNum = QuadrantCondition(HitQuadrant.GetComponent()->GetName());
	switch (QuadrantConditionNum)
	{
		case 1: //Top
			{
				HitActorSnapLocation = Cast<ATimberBuildingComponentBase>(HitActor.GetActor())->TopSnap->GetComponentTransform().GetLocation();
				ProxySnapLocation = ActiveBuildingComponent->BottomSnap->GetComponentTransform().GetLocation();
				MoveProxyToSnapLocation(ProxySnapLocation, HitActorSnapLocation);
			}
			break;
		case 2: //Right
			{
				HitActorSnapLocation = Cast<ATimberBuildingComponentBase>(HitActor.GetActor())->RightSnap->GetComponentTransform().GetLocation();
				ProxySnapLocation = ActiveBuildingComponent->LeftSnap->GetComponentTransform().GetLocation();
				MoveProxyToSnapLocation(ProxySnapLocation, HitActorSnapLocation);
			}
		break;
	case 3: //Bottom
			{
				HitActorSnapLocation = Cast<ATimberBuildingComponentBase>(HitActor.GetActor())->BottomSnap->GetComponentTransform().GetLocation();
				ProxySnapLocation = ActiveBuildingComponent->TopSnap->GetComponentTransform().GetLocation();
				MoveProxyToSnapLocation(ProxySnapLocation, HitActorSnapLocation);
			}
			break;
		case 4: //Left
			{
				HitActorSnapLocation = Cast<ATimberBuildingComponentBase>(HitActor.GetActor())->LeftSnap->GetComponentTransform().GetLocation();
				ProxySnapLocation = ActiveBuildingComponent->RightSnap->GetComponentTransform().GetLocation();
				MoveProxyToSnapLocation(ProxySnapLocation, HitActorSnapLocation);
			}
			break;
		case 5: //Center
			{
				//TODO:: What should we do here? Center Quad.
			}
			break;
		default:
			{
				UE_LOG(LogTemp, Error, TEXT("Quadrant Not Specified. "))
			}
		break;
	}
}

void UBuildSystemManagerComponent::HorizontalSnapCondition(FHitResult HitActor, FHitResult HitQuadrant)
{
}

void UBuildSystemManagerComponent::VerticalToHorizontalSnapCondition(FHitResult HitActor, FHitResult HitQuadrant)
{
}

void UBuildSystemManagerComponent::HorizontalToVerticalSnapCondition(FHitResult HitActor, FHitResult HitQuadrant)
{
}

void UBuildSystemManagerComponent::MoveProxyToSnapLocation(FVector ProxySnapLocation, FVector SnapLocation)
{
	FVector MoveLocation = SnapLocation - ProxySnapLocation;
	ActiveBuildingComponent->SetActorLocation(MoveLocation);
	
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



