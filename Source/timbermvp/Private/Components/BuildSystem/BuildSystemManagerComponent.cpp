// Property of Paracosm Industries. Dont use my shit.


#include "Components/BuildSystem/BuildSystemManagerComponent.h"
#include "BuildSystem/BuildingComponents/TimberBuildingComponentBase.h"
#include "BuildSystem/BuildingComponents/TimberHorizontalBuildingComponent.h"
#include "BuildSystem/BuildingComponents/TimberVerticalBuildingComponent.h"
#include "BuildSystem/Traps/TrapBase.h"
#include "BuildSystem/BuildableBase.h"
#include "BuildSystem/Ramps/RampBase.h"

UBuildSystemManagerComponent::UBuildSystemManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UBuildSystemManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

/*Component to Component Snapping*/
void UBuildSystemManagerComponent::HandleBuildingComponentSnapping(FHitResult HitQuadrant, FHitResult HitActor)
{
	if (ActiveBuildingComponentProxy)
	{
		const EBuildingComponentOrientation ProxyBuildingComponentOrientation = ActiveBuildingComponentProxy->
			BuildingOrientation;
		// Get HitResult Building Component Enum Orientation
		EBuildingComponentOrientation PlacedBuildingComponentOrientation = CheckClassBuildingComponentOrientation(
			HitActor.GetActor());

		const FString HitQuadrantName = *HitQuadrant.GetComponent()->GetName();
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *HitQuadrantName);

		// Returns the condition to snap based on Orientation
		int SnappingConditionNum = SnappingCondition(
			ProxyBuildingComponentOrientation, PlacedBuildingComponentOrientation);

		//Handles the Movement
		switch (SnappingConditionNum)
		{
		case 1: SameOrientationSnapCondition(HitActor, HitQuadrant); //vertical - vertical
			break;
		case 2: SameOrientationSnapCondition(HitActor, HitQuadrant); // horizontal - horizontal
			break;
		case 3: VerticalToHorizontalSnapCondition(HitActor, HitQuadrant);
			break;
		case 4: HorizontalToVerticalSnapCondition(HitActor, HitQuadrant);
			break;
		default: UE_LOG(LogTemp, Error, TEXT("Error handling Orientation Condition Check."))
		}
	}
}

int UBuildSystemManagerComponent::SnappingCondition(
	EBuildingComponentOrientation Orientation1, EBuildingComponentOrientation Orientation2)
{
	if (Orientation1 == Orientation2 && Orientation1 == EBuildingComponentOrientation::Vertical)
	{
		//Both Vertical
		return 1;
	}

	if (Orientation1 == Orientation2 && Orientation1 == EBuildingComponentOrientation::Horizontal)
	{
		//Both Horizontal
		return 2;
	}

	if (Orientation1 != Orientation2 && Orientation1 == EBuildingComponentOrientation::Vertical)
	{
		//Orientation 1 = Vertical, Orientation 2 = Horizontal
		return 3;
	}

	if (Orientation1 != Orientation2 && Orientation1 == EBuildingComponentOrientation::Horizontal)
	{
		//Orientation 1 = Horizontal, Orientation 2 = Vertical
		return 3;
	}

	return 4;
}

int UBuildSystemManagerComponent::QuadrantCondition(FString QuadrantName)
{
	if (QuadrantName == FString("TopQuadrant"))
	{
		return 1;
	}
	if (QuadrantName == FString("RightQuadrant"))
	{
		return 2;
	}
	if (QuadrantName == FString("BottomQuadrant"))
	{
		return 3;
	}
	if (QuadrantName == FString("LeftQuadrant"))
	{
		return 4;
	}
	if (QuadrantName == FString("CenterQuadrant"))
	{
		return 5;
	}

	return 6;
}

void UBuildSystemManagerComponent::SameOrientationSnapCondition(FHitResult HitActor, FHitResult HitQuadrant)
{
	FVector ProxySnapLocation;
	FVector HitActorSnapLocation;

	int QuadrantConditionNum = QuadrantCondition(HitQuadrant.GetComponent()->GetName());
	switch (QuadrantConditionNum)
	{
	case 1: //Top
		{
			HitActorSnapLocation = Cast<ATimberBuildingComponentBase>(HitActor.GetActor())->TopSnap->
				GetComponentTransform().GetLocation();
			ProxySnapLocation = ActiveBuildingComponentProxy->BottomSnap->GetComponentTransform().GetLocation();
			MoveProxyToSnapLocation(ProxySnapLocation, HitActorSnapLocation);
		}
		break;
	case 2: //Right
		{
			HitActorSnapLocation = Cast<ATimberBuildingComponentBase>(HitActor.GetActor())->RightSnap->
				GetComponentTransform().GetLocation();
			ProxySnapLocation = ActiveBuildingComponentProxy->LeftSnap->GetComponentTransform().GetLocation();
			MoveProxyToSnapLocation(ProxySnapLocation, HitActorSnapLocation);
		}
		break;
	case 3: //Bottom
		{
			HitActorSnapLocation = Cast<ATimberBuildingComponentBase>(HitActor.GetActor())->BottomSnap->
				GetComponentTransform().GetLocation();
			ProxySnapLocation = ActiveBuildingComponentProxy->TopSnap->GetComponentTransform().GetLocation();
			MoveProxyToSnapLocation(ProxySnapLocation, HitActorSnapLocation);
		}
		break;
	case 4: //Left
		{
			HitActorSnapLocation = Cast<ATimberBuildingComponentBase>(HitActor.GetActor())->LeftSnap->
				GetComponentTransform().GetLocation();
			ProxySnapLocation = ActiveBuildingComponentProxy->RightSnap->GetComponentTransform().GetLocation();
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

void UBuildSystemManagerComponent::RotateProxyToSnapRotation(
	FRotator HitActorRotation, ABuildableBase* BuildingComponent)
{
	BuildingComponent->SetActorRotation(HitActorRotation);
}

void UBuildSystemManagerComponent::VerticalToHorizontalSnapCondition(FHitResult HitActor, FHitResult HitQuadrant)
{
	int QuadrantConidtion = QuadrantCondition(HitQuadrant.GetComponent()->GetName());
	FVector HitActorSnapLocation;
	FRotator HitActorSnapRotation;
	FVector ProxySnapLocation;
	switch (QuadrantConidtion)
	{
	case 1:
		{
			//Bottom Vertical SnapPoint -> Top Quad on Horizontal
			HitActorSnapLocation = Cast<ATimberBuildingComponentBase>(HitActor.GetActor())->TopSnap->
				GetComponentTransform().GetLocation();
			HitActorSnapRotation = Cast<ATimberBuildingComponentBase>(HitActor.GetActor())
			                       ->TopSnap->GetComponentTransform().GetRotation().Rotator();
			ProxySnapLocation = ActiveBuildingComponentProxy->BottomSnap->GetComponentTransform().GetLocation();
			MoveProxyToSnapLocation(ProxySnapLocation, HitActorSnapLocation);
		}
		break;
	case 2:
		{
			//Bottom Vertical SnapPoint -> Right Quad on Horizontal
			HitActorSnapLocation = Cast<ATimberBuildingComponentBase>(HitActor.GetActor())
			                       ->RightSnap->GetComponentTransform().GetLocation();
			HitActorSnapRotation = Cast<ATimberBuildingComponentBase>(HitActor.GetActor())
			                       ->RightSnap->GetComponentTransform().GetRotation().Rotator();
			ProxySnapLocation = ActiveBuildingComponentProxy->BottomSnap->GetComponentTransform().GetLocation();
			MoveProxyToSnapLocation(ProxySnapLocation, HitActorSnapLocation);
		}
		break;
	case 3:
		{
			//Bottom Vertical SnapPoint -> Bottom Quad on Horizontal
			HitActorSnapLocation = Cast<ATimberBuildingComponentBase>(HitActor.GetActor())
			                       ->BottomSnap->GetComponentTransform().GetLocation();
			HitActorSnapRotation = Cast<ATimberBuildingComponentBase>(HitActor.GetActor())
			                       ->BottomSnap->GetComponentTransform().GetRotation().Rotator();
			ProxySnapLocation = ActiveBuildingComponentProxy->BottomSnap->GetComponentTransform().GetLocation();
			MoveProxyToSnapLocation(ProxySnapLocation, HitActorSnapLocation);
		}
		break;
	case 4:
		{
			//Bottom Vertical SnapPoint -> Left Quad on Horizontal
			HitActorSnapLocation = Cast<ATimberBuildingComponentBase>(HitActor.GetActor())
			                       ->LeftSnap->GetComponentTransform().GetLocation();
			HitActorSnapRotation = Cast<ATimberBuildingComponentBase>(HitActor.GetActor())
			                       ->LeftSnap->GetComponentTransform().GetRotation().Rotator();
			ProxySnapLocation = ActiveBuildingComponentProxy->BottomSnap->GetComponentTransform().GetLocation();
			MoveProxyToSnapLocation(ProxySnapLocation, HitActorSnapLocation);
		}
		break;
	case 5:
		{
			//Bottom Vertical SnapPoint -> Center Quad on Horizontal
			HitActorSnapLocation = Cast<ATimberBuildingComponentBase>(HitActor.GetActor())
			                       ->CenterSnap->GetComponentTransform().GetLocation();
			HitActorSnapRotation = Cast<ATimberBuildingComponentBase>(HitActor.GetActor())
			                       ->CenterSnap->GetComponentTransform().GetRotation().Rotator();
			ProxySnapLocation = ActiveBuildingComponentProxy->BottomSnap->GetComponentTransform().GetLocation();
			MoveProxyToSnapLocation(ProxySnapLocation, HitActorSnapLocation);
			//No Snap Rotation, Player will Rotate on their own.
		}
		break;
	default:
		{
			UE_LOG(LogTemp, Error, TEXT("No Matching Snap Condition from Vertical to Horizontal."))
		}
	}
}

void UBuildSystemManagerComponent::HorizontalToVerticalSnapCondition(FHitResult HitActor, FHitResult HitQuadrant)
{
	int QuadrantCondition1 = QuadrantCondition(HitQuadrant.GetComponent()->GetName());
	FVector HitActorSnapLocation;
	FVector ProxySnapLocation;

	switch (QuadrantCondition1)
	{
	case 1:
		{
			//Bottom Vertical SnapPoint -> Top Quad on Horizontal
			HitActorSnapLocation = Cast<ATimberBuildingComponentBase>(HitActor.GetActor())->TopSnap->
				GetComponentTransform().GetLocation();
			ProxySnapLocation = ActiveBuildingComponentProxy->TopSnap->GetComponentTransform().GetLocation();
			MoveProxyToSnapLocation(ProxySnapLocation, HitActorSnapLocation);
		}
		break;
	case 3:
		{
			//Bottom Vertical SnapPoint -> Top Quad on Horizontal
			HitActorSnapLocation = Cast<ATimberBuildingComponentBase>(HitActor.GetActor())
			                       ->BottomSnap->GetComponentTransform().GetLocation();
			ProxySnapLocation = ActiveBuildingComponentProxy->TopSnap->GetComponentTransform().GetLocation();
			MoveProxyToSnapLocation(ProxySnapLocation, HitActorSnapLocation);
		}
		break;
	default: UE_LOG(LogTemp, Error, TEXT("Floors and Ceilings can not Attach to Sides of Walls."));
	}
}

void UBuildSystemManagerComponent::MoveProxyToSnapLocation(FVector ProxySnapLocation, FVector SnapLocation)
{
	//Returns a vector
	FVector MoveLocation = SnapLocation - ProxySnapLocation;
	FVector CurrentLocation = ActiveBuildingComponentProxy->GetActorLocation();
	ActiveBuildingComponentProxy->SetActorLocation(CurrentLocation + MoveLocation);
}

EBuildingComponentOrientation UBuildSystemManagerComponent::CheckClassBuildingComponentOrientation(
	AActor* ClassToBeChecked)
{
	if (Cast<ATimberVerticalBuildingComponent>(ClassToBeChecked))
	{
		return Cast<ATimberVerticalBuildingComponent>(ClassToBeChecked)->BuildingOrientation;
	}
	if (Cast<ATimberHorizontalBuildingComponent>(ClassToBeChecked))
	{
		return Cast<ATimberHorizontalBuildingComponent>(ClassToBeChecked)->BuildingOrientation;
	}
	return EBuildingComponentOrientation::Default;
}

/*Spawn Setup*/
FVector UBuildSystemManagerComponent::SnapToGrid(FVector RaycastLocation)
{
	FVector SnappedVector;
	const int SnappedX = (FMath::FloorToInt(RaycastLocation.X) / GridSize) * GridSize;
	const int SnappedY = (FMath::FloorToInt(RaycastLocation.Y) / GridSize) * GridSize;

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
	if (SavedRotation != FRotator::ZeroRotator)
	{
		return SavedRotation;
	}
	float CharacterRotationYaw = CharactersRotation.Yaw;

	// I think the intent here was to always make the Spawned Proxy and the Spawned Component Face the player when spawned.
	if (CharacterRotationYaw <= 45 && CharacterRotationYaw >= 0)
	{
		SavedRotation.Yaw = 180;
	}
	else if (CharacterRotationYaw >= -45 && CharacterRotationYaw < 0)
	{
		SavedRotation.Yaw = 180;
	}
	else if (CharacterRotationYaw > 45 && CharacterRotationYaw <= 135)
	{
		SavedRotation.Yaw = -90;
	}
	else if (CharacterRotationYaw < -45 && CharacterRotationYaw > -135)
	{
		SavedRotation.Yaw = 90;
	}
	else if (CharacterRotationYaw > 135)
	{
		SavedRotation.Yaw = 0;
	}
	else if (CharacterRotationYaw <= -135)
	{
		SavedRotation.Yaw = 0;
	}

	FinalSpawnRotation = SavedRotation;

	return SavedRotation;
}

// Returns the closest snap location based on the impact point of the raycast for the building component.
FTrapSnapData UBuildSystemManagerComponent::GetTrapSnapTransform(
	FVector ImpactPoint, ATimberBuildingComponentBase* BuildingComponent, ATrapBase* TrapComponent)
{
	// Default Snap Data is facing the player at the impact point
	FTrapSnapData TrapSnapData;
	TrapSnapData.TrapLocation = ImpactPoint;
	TrapSnapData.TrapRotation = GetOwner()->GetActorRotation();
	TrapSnapData.TrapRotation.Yaw += 90;

	if (BuildingComponent->FrontTrapSnap && BuildingComponent->BackTrapSnap)
	{
		/* Getting Distance from impact point to the closest scene component
		 * that would always give the side of the wall/floor/ceiling's Snap location
		 */

		FVector FrontTrapSnapLocation = BuildingComponent->FrontTrapSnap->GetComponentTransform().GetLocation();
		FVector BackTrapSnapLocation = BuildingComponent->BackTrapSnap->GetComponentTransform().GetLocation();
		float LengthToFrontTrapSnap = FVector::Dist(ImpactPoint, FrontTrapSnapLocation);
		float LengthToBackTrapSnap = FVector::Dist(ImpactPoint, BackTrapSnapLocation);

		if (LengthToFrontTrapSnap < LengthToBackTrapSnap)
		{
			//If the FrontSnap is empty, assign the TrapComponent to the FrontSnap, else do not snap to component.
			if (BuildingComponent->FrontTrap == nullptr)
			{
				TrapSnapData.TrapLocation = FrontTrapSnapLocation;
				TrapSnapData.TrapRotation = BuildingComponent->FrontTrapSnap->GetComponentTransform().GetRotation().
				                                               Rotator();
				TrapComponent->BuildingComponentTrapDirection = EBuildingComponentTrapDirection::Front;
				TrapComponent->SetCanTrapBeFinalized(true);
			}
			else
			{
				TrapComponent->SetCanTrapBeFinalized(false);
				TrapComponent->BuildingComponentTrapDirection = EBuildingComponentTrapDirection::Default;
				GEngine->AddOnScreenDebugMessage(7, 5.0f, FColor::Red, "FrontSnap Taken.");
			}
		}
		else
		{
			if (BuildingComponent->BackTrap == nullptr)
			{
				TrapSnapData.TrapLocation = BackTrapSnapLocation;
				TrapSnapData.TrapRotation = BuildingComponent->BackTrapSnap->GetComponentTransform().GetRotation().
				                                               Rotator();
				TrapComponent->BuildingComponentTrapDirection = EBuildingComponentTrapDirection::Back;
				TrapComponent->SetCanTrapBeFinalized(true);
			}
			else
			{
				TrapComponent->SetCanTrapBeFinalized(false);
				TrapComponent->BuildingComponentTrapDirection = EBuildingComponentTrapDirection::Default;
				GEngine->AddOnScreenDebugMessage(7, 5.0f, FColor::Red, "BackSnap Taken.");
			}
		}
	}
	else
	{
		TrapComponent->SetCanTrapBeFinalized(false);
		TrapComponent->BuildingComponentTrapDirection = EBuildingComponentTrapDirection::Default;
	}
	return TrapSnapData;
}

/* Material Shading */
void UBuildSystemManagerComponent::HandleTrapMaterialChange(bool bCanTrapBeFinalized)
{
	if (bCanTrapBeFinalized)
	{
		GEngine->AddOnScreenDebugMessage(3, 4.0, FColor::Red, "Trap Can Be Finalized. Setting Color to Blue.");
		MakeMaterialHoloColor(ActiveTrapComponentProxy, BlueHoloMaterial);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(3, 4.0, FColor::Red, "Trap Cant Be Finalized. Setting Color to Red.");
		MakeMaterialHoloColor(ActiveTrapComponentProxy, RedHoloMaterial);
	}
}

void UBuildSystemManagerComponent::RegisterTrapComponent(ATrapBase* TrapComponent)
{
	/* When the trap changes its finalization capability, this callback will happen. This changes the color from red to blue depending on finalization capabilities*/
	if (TrapComponent)
	{
		TrapComponent->OnTrapFinalizationChange.AddDynamic(
			this, &UBuildSystemManagerComponent::HandleTrapMaterialChange);
		HandleTrapMaterialChange(TrapComponent->GetCanTrapBeFinalized());
	}
}

//May be removed in the future for alternate MakeMaterialHoloColor function. V
void UBuildSystemManagerComponent::MakeBuildingComponentProxy(AActor* BuildingComponentProxy)
{
	//Get the Static MeshComponent of the passed in Building Component
	UStaticMeshComponent* MeshComponent = BuildingComponentProxy->FindComponentByClass<UStaticMeshComponent>();

	if (MeshComponent)
	{
		{
			//Creating a Material Instance if one doesn't exist already.
			UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(
				MeshComponent->GetMaterial(1),
				this);

			//Make the Opacity of the Material 0.5f
			if (MaterialInstance)
			{
				GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, "There is a valid Material Instance.");
				//Parameter Created in the Material Instance
				// These are Parameter Nodes that are Created and Defaulted by us in the Material Editor.
				//MaterialInstance->BlendMode = BLEND_Translucent;
				MaterialInstance->SetScalarParameterValue("Opacity", GhostOpacity);
				//Can be made Red in the future for unavailable build location.
				MaterialInstance->SetVectorParameterValue("EmissiveColor", FLinearColor(0.0f, 0.0f, 1.0f, 1.0f));
				MeshComponent->SetMaterial(1, MaterialInstance);
			}
		}
	}
}

void UBuildSystemManagerComponent::GetStaticMeshComponents(AActor* BuildingComponentActor)
{
	if (BuildingComponentActor)
	{
		//Gets static Meshes and returns them to the GlobalArray.
		BuildingComponentActor->GetComponents<UStaticMeshComponent>(StaticMeshs);
	}
}

void UBuildSystemManagerComponent::MakeMaterialHoloColor(AActor* BuildingComponentActor, UMaterial* HoloMaterialColor)
{
	if (StaticMeshs.Num() == 0)
	{
		GetStaticMeshComponents(BuildingComponentActor);
	}

	// Get all Static Meshes from the Actor
	if (StaticMeshs.Num() > 0)
	{
		GEngine->AddOnScreenDebugMessage(5, 3.0f, FColor::Green, "Changing mesh color now.");
		for (UStaticMeshComponent* MeshComponent : StaticMeshs)
		{
			if (MeshComponent && MeshComponent->GetStaticMesh())
			{
				TArray<FStaticMaterial> MaterialSlots = MeshComponent->GetStaticMesh()->GetStaticMaterials();
				for (int Index = 0; Index < MaterialSlots.Num(); Index++)
				{
					if (HoloMaterialColor)
					{
						MeshComponent->SetMaterial(Index, HoloMaterialColor);
					}
				}
			}
		}
	}
}

/* Buildable Placement */
void UBuildSystemManagerComponent::HandleRampPlacement(TArray<FHitResult> HitResults)
{
	//Just to get Here the Raycast must have hit something.

	FVector_NetQuantize Location = HitResults[0].ImpactPoint;
	FActorSpawnParameters SpawnParameters;

	//SPAWNING TRAP COMPONENT
	//TODO:: This type of function is used repeatedly. Can we make a function that handles this?
	if (ActiveRampComponentProxy == nullptr || ActiveRampComponentProxy->GetClass() != GetActiveBuildableClass())
	{
		//Spawn the first iterations of the Ramp into the world.
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>
		(
			ActiveBuildableComponentClass,
			Location,
			FRotator::ZeroRotator,
			SpawnParameters);
		ARampBase* SpawnedRamp = Cast<ARampBase>(SpawnedActor);
		BuildableRef = Cast<ABuildableBase>(SpawnedActor);
		if (SpawnedRamp)
		{
			SetActiveRampComponent(SpawnedRamp);
			SpawnedRamp->SetActorEnableCollision(false);
		}
	}

	//Getting the First Hit Building Component
	/*
	 * Get the first Hit Building Component.
	 * Find vector Difference (Offset) between the Ramps Center Snap and the Building Components Center Snap. (Horizontal or Vertical)
	 * Move the Ramp the Offset Amount so that the Ramps Center Snap is at the Building Components Center Snap.
	 */
	ATimberBuildingComponentBase* FirstHitBuildingComponent = nullptr;
	for (const FHitResult& Hits : HitResults)
	{
		//If the Hit Actor is a Building Component
		if (Cast<ATimberBuildingComponentBase>(Hits.GetActor()))
		{
			FirstHitBuildingComponent = Cast<ATimberBuildingComponentBase>(Hits.GetActor());
			break;
		}
	}

	//LOCATION PLACEMENT OF THE RAMP
	if (FirstHitBuildingComponent && ActiveRampComponentProxy)
	{
		if (FirstHitBuildingComponent->BuildingOrientation == EBuildingComponentOrientation::Vertical)
		{
			// Snap Ramps Vertical Snap to the Building Components Vertical Center Snap
			FVector RampVerticalCenterSnap = ActiveRampComponentProxy->VerticalCenterSnap->GetComponentLocation();
			FVector HitBuildingCenterSnap = FirstHitBuildingComponent->CenterSnap->GetComponentLocation();
			FVector OffsetVector = HitBuildingCenterSnap - RampVerticalCenterSnap;
			ActiveRampComponentProxy->SetActorLocation(ActiveRampComponentProxy->GetActorLocation() + OffsetVector);
			MakeMaterialHoloColor(ActiveRampComponentProxy, BlueHoloMaterial);
			ActiveRampComponentProxy->SetRampFinalization(true);
		}
		else if (FirstHitBuildingComponent->BuildingOrientation == EBuildingComponentOrientation::Horizontal)
		{
			// Snap Ramps Horizontal Snap to the Building Components Horizontal Center Snap
			FVector RampHorizontalCenterSnap = ActiveRampComponentProxy->HorizontalCenterSnap->GetComponentLocation();
			FVector HitBuildingCenterSnap = FirstHitBuildingComponent->CenterSnap->GetComponentLocation();
			FVector OffsetVector = HitBuildingCenterSnap - RampHorizontalCenterSnap;
			ActiveRampComponentProxy->SetActorLocation(ActiveRampComponentProxy->GetActorLocation() + OffsetVector);
			MakeMaterialHoloColor(ActiveRampComponentProxy, BlueHoloMaterial);
			ActiveRampComponentProxy->SetRampFinalization(true);
		}
	}
	else //Hit but not a building Component that is snappable.
	{
		MakeMaterialHoloColor(GetActiveRampComponent(), RedHoloMaterial);
		ActiveRampComponentProxy->SetActorLocation(Location);
		ActiveRampComponentProxy->SetRampFinalization(false);
	}
}

/* Spawn */
void UBuildSystemManagerComponent::SpawnFinalBuildingComponent()
{
	FActorSpawnParameters SpawnParameters;

	if(ActiveBuildableComponentClass)
	{
		if (ActiveBuildableComponentClass->IsChildOf(ATrapBase::StaticClass()))
		{
			SpawnFinalTrap(SpawnParameters);
		}
		else if (ActiveBuildableComponentClass->IsChildOf(ATimberBuildingComponentBase::StaticClass()))
		{
			SpawnFinalBuildingComponent(SpawnParameters);
		}
		else if (ActiveBuildableComponentClass->IsChildOf(ARampBase::StaticClass()))
		{
			SpawnFinalRampComponent();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(4, 3.0f, FColor::Magenta, "No Active Buildable Class.");
		}
	}

	
}

void UBuildSystemManagerComponent::SpawnFinalRampComponent()
{
	if (ActiveRampComponentProxy && ActiveRampComponentProxy->GetRampFinalization())
	{
		//Spawn Final Ramp at the Component Proxies location
		FActorSpawnParameters SpawnParams;
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>
		(
			ActiveBuildableComponentClass,
			ActiveRampComponentProxy->GetActorLocation(),
			ActiveRampComponentProxy->GetActorRotation(),
			SpawnParams);
	}

	RedrawPathTraceHandle.Broadcast();
}

void UBuildSystemManagerComponent::SpawnBuildingComponentProxy(FVector SpawnVector, FRotator SpawnRotator)
{
	if (ActiveBuildableComponentClass)
	{
		//TODO:: Remember to change this back in the future.
		const FVector Location = SnapToGrid(SpawnVector);
		const FVector Location1 = SpawnVector;
		const FRotator Rotation = SnapToRotation(SpawnRotator);
		const FActorSpawnParameters SpawnParameters;

		//Use the InputTransform as the Location to Spawn the ActiveBuildingComponent
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>
		(
			ActiveBuildableComponentClass,
			Location1,
			Rotation,
			SpawnParameters);
		
		ActiveBuildingComponentProxy = Cast<ATimberBuildingComponentBase>(SpawnedActor);
		BuildableRef = Cast<ABuildableBase>(SpawnedActor);

		ActiveBuildingComponentProxy->SetActorEnableCollision(false);

		//Make the Building Component have the "see-through" material look
		MakeBuildingComponentProxy(SpawnedActor);
	}
}

void UBuildSystemManagerComponent::SpawnTrapComponentProxy(FVector_NetQuantize Location, FRotator SpawnRotator)
{
	//Clearing any existing stored Static Meshes from Previous Trap Proxies
	ClearStoredStaticMeshes();

	FActorSpawnParameters SpawnParameters;
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>
	(
		ActiveBuildableComponentClass,
		Location,
		SpawnRotator,
		SpawnParameters);
	if (SpawnedActor)
	{
		ActiveTrapComponentProxy = Cast<ATrapBase>(SpawnedActor);
		//Binding the Delegate Call to the newly Spawned Trap Component
		RegisterTrapComponent(ActiveTrapComponentProxy);
	}
}

bool UBuildSystemManagerComponent::SpawnFinalTrap(FActorSpawnParameters SpawnParameters)
{
	if (ActiveTrapComponentProxy && ActiveTrapComponentProxy->GetCanTrapBeFinalized())
	{
		//Spawn Final Trap
		GEngine->AddOnScreenDebugMessage(5, 3, FColor::Black, "Trap Finalized and spawned.");
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>
		(
			ActiveBuildableComponentClass,
			ActiveTrapComponentProxy->GetActorLocation(),
			ActiveTrapComponentProxy->GetActorRotation(),
			SpawnParameters);
		
		if (SpawnedActor && ActiveTrapComponentProxy && ActiveTrapComponentProxy->HoveredBuildingComponent)
		{
			EBuildingComponentTrapDirection TrapDirection = ActiveTrapComponentProxy->BuildingComponentTrapDirection;
			switch (TrapDirection)
			{
			case EBuildingComponentTrapDirection::Front:
				ActiveTrapComponentProxy->HoveredBuildingComponent->FrontTrap = Cast<ATrapBase>(SpawnedActor);
				break;
			case EBuildingComponentTrapDirection::Back:
				ActiveTrapComponentProxy->HoveredBuildingComponent->BackTrap = Cast<ATrapBase>(SpawnedActor);
				break;
			case EBuildingComponentTrapDirection::Default:
				GEngine->AddOnScreenDebugMessage(3, 3.0f, FColor::Red, "Trap Direction Not Specified.");
				return true;
			}
		}
		//Needed to Replace the existing TrapComponentProxy with the new TrapComponent Proxy
		ResetBuildableComponents(ATrapBase::StaticClass());
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(
			8, 5.0f, FColor::Red, "Trap Cannot Be Finalized. BuildSystemManager-SpawnFinalBuildingComponent");
	}
	return false;
}

// TODO:: This function is not used. Remove in the future. 
void UBuildSystemManagerComponent::SpawnFinalBuildingComponent(FActorSpawnParameters SpawnParameters)
{
	//Use the InputTransform as the Location to Spawn the ActiveBuildingComponent
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>
	(
		ActiveBuildableComponentClass,
		ActiveBuildingComponentProxy->GetActorLocation(),
		ActiveBuildingComponentProxy->GetActorRotation(),
		SpawnParameters);
	SpawnedActor->SetActorEnableCollision(true);
	RedrawPathTraceHandle.Broadcast();
}

void UBuildSystemManagerComponent::MoveBuildingComponent(
	FVector_NetQuantize Location, ABuildableBase*
	BuildingComponent, const FRotator& Rotation)
{
	if (BuildingComponent)
	{
		//OnHits we will pass in the Rotation of the HitActor so the Components Match its Rotation
		if (Rotation != FRotator::ZeroRotator)
		{
			GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, "Rotated Trap to match Building Component.");
			BuildingComponent->SetActorRotation(Rotation);
		}
		//REMEMBER to change this back in the future if you want some snapping.
		//ActiveBuildingComponent->SetActorLocation(SnapToGrid(Location));

		BuildingComponent->SetActorLocation(Location);
		
	}
}

void UBuildSystemManagerComponent::ResetBuildableComponents(TSubclassOf<ABuildableBase> ActiveBuildableClass)
{
	if (ActiveBuildableClass->IsChildOf(ATimberBuildingComponentBase::StaticClass()))
	{
		if (ActiveBuildingComponentProxy)
		{
			ActiveBuildingComponentProxy->Destroy();
			ActiveBuildingComponentProxy = nullptr;
		}
	}

	if (ActiveBuildableClass->IsChildOf(ATrapBase::StaticClass()))
	{
		if (ActiveTrapComponentProxy)
		{
			ActiveTrapComponentProxy->Destroy();
			ActiveTrapComponentProxy = nullptr;
		}
	}

	if (ActiveBuildableClass->IsChildOf(ARampBase::StaticClass()))
	{
		if (ActiveRampComponentProxy)
		{
			ActiveRampComponentProxy->Destroy();
			ActiveRampComponentProxy = nullptr;
		}
	}

	SetBuildableRefToNull();
	StaticMeshs.Empty();
}

void UBuildSystemManagerComponent::RemoveBuildingComponentProxies_All()
{
	ResetBuildableComponents(ATrapBase::StaticClass());
	ResetBuildableComponents(ATimberBuildingComponentBase::StaticClass());
	ResetBuildableComponents(ARampBase::StaticClass());
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			3, 5.0f, FColor::Green, "ATimberPlayableCharacter::ExitBuildMode() : Building Component Proxy Removed. ");
	}
}

/*Input Callbacks*/
void UBuildSystemManagerComponent::RotateBuildingComponent()
{
	if (BuildableRef)
	{
		SavedRotation.Yaw += 90;
		if (SavedRotation.Yaw >= 360) // Reset Yaw to 0 if it reaches 360
		{
			SavedRotation.Yaw = 0;
		}
		BuildableRef->SetActorRotation(SavedRotation);

		//Updating FinalSpawnRotation after Player Rotates Component
		FinalSpawnRotation = SavedRotation;
	}
}

void UBuildSystemManagerComponent::SetActiveBuildingComponentClass(TSubclassOf<AActor> BuildingComponentClass)
{
	ActiveBuildableComponentClass = BuildingComponentClass;
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 3.0, FColor::Green, "Active Building Component Class Set.");
	}
}

