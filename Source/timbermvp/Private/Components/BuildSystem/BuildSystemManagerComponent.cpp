// Property of Paracosm Industries. Dont use my shit.


#include "Components/BuildSystem/BuildSystemManagerComponent.h"


#include "BuildSystem/BuildingComponents/TimberBuildingComponentBase.h"
#include "BuildSystem/BuildingComponents/TimberHorizontalBuildingComponent.h"
#include "BuildSystem/BuildingComponents/TimberVerticalBuildingComponent.h"
#include "BuildSystem/Traps/TrapBase.h"
#include "BuildSystem/BuildableBase.h"
#include "BuildSystem/Constructs/PowerPlate.h"
#include "BuildSystem/Constructs/TeleportConstruct.h"
#include "BuildSystem/Ramps/RampBase.h"
#include "Character/TimberPlayableCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/Inventory/InventoryManagerComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "States/DieRobotGameStateBase.h"
#include "Subsystems/SFX/SFXManagerSubsystem.h"

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
		//Gets Proxy Building Component Enum Orientation
		const EBuildingComponentOrientation ProxyBuildingComponentOrientation = ActiveBuildingComponentProxy->
			BuildingOrientation;
		
		// Get HitResult Building Component Enum Orientation
		EBuildingComponentOrientation PlacedBuildingComponentOrientation = CheckClassBuildingComponentOrientation(
			HitActor.GetActor());

		//const FString HitQuadrantName = *HitQuadrant.GetComponent()->GetName();
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *HitQuadrantName);

		// Returns the condition to snap based on Orientation
		int SnappingConditionNum = SnappingCondition(
			ProxyBuildingComponentOrientation, PlacedBuildingComponentOrientation);

		/*
		 * Based on the 2 component orientations, we can determine the snapping conditions
		 */
		switch (SnappingConditionNum)
		{
		case 1:
			SameOrientationSnapCondition(HitActor, HitQuadrant); //Wall Proxy to Wall
			//UE_LOG(LogTemp, Warning, TEXT("Vertical to Vertical Snap Condition."))
			break;
		case 2:
			SameOrientationSnapCondition(HitActor, HitQuadrant); // Floor Proxy to Floor
			//UE_LOG(LogTemp, Warning, TEXT("Horizontal to Horizontal Snap Condition."));
			break;
		case 3:
			VerticalToHorizontalSnapCondition(HitActor, HitQuadrant); //Wall Proxy to Floor
			//UE_LOG(LogTemp, Warning, TEXT("Horizontal to Vertical Snap Condition."));
			break;
		case 4:
			HorizontalToVerticalSnapCondition(HitActor, HitQuadrant); //Floor Proxy to Wall
			//UE_LOG(LogTemp, Warning, TEXT("Vertical to Horizontal Snap Condition."));
			break;
		default:
			//UE_LOG(LogTemp, Error, TEXT("Error handling Orientation Condition Check."));
			break;
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
	//Returns a case for the Overlapped Quadrant of the Raycast 
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
			//UE_LOG(LogTemp, Error, TEXT("Quadrant Not Specified. "))
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
			//UE_LOG(LogTemp, Error, TEXT("No Matching Snap Condition from Vertical to Horizontal."))
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
	FVector MoveLocation = SnapLocation - ProxySnapLocation;
	FVector CurrentLocation = ActiveBuildingComponentProxy->GetActorLocation();
	ActiveBuildingComponentProxy->SetActorLocation(CurrentLocation + MoveLocation);

	FVector WorldLocation = ActiveBuildingComponentProxy->GetActorTransform().TransformPosition(MoveLocation);

	//Used for Walls. We dont want to trace at the base of the wall, we want to trace at the center of the wall.
	//For Floors we want to trace at the base.
	if (Cast<ATimberVerticalBuildingComponent>(ActiveBuildingComponentProxy))
	{
		WorldLocation.Z += 200.0f;
	}

	//Checks if there is a Building Component already at the Snapped Location by Using an Overlap box as a test.
	TArray<FOverlapResult> Overlaps;
	bool bWillOverlap = GetWorld()->OverlapMultiByChannel(Overlaps, WorldLocation, FQuat::Identity, 
	ECC_GameTraceChannel1, 
	FCollisionShape::MakeBox(FVector(40, 40, 40)));

	//DrawDebugBox(GetWorld(), WorldLocation, FVector(40, 40, 40), FColor::Green, false, -1);
	//UE_LOG(LogTemp, Warning, TEXT("Moving Building Component."));
	
	if (bWillOverlap)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Building Component Will Overlap."));
		ActiveBuildingComponentProxy->bCanBuildableBeFinalized = false;
		MakeMaterialHoloColor(ActiveBuildingComponentProxy, RedHoloMaterial);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Building Component Will Not Overlap."));
		ActiveBuildingComponentProxy->bCanBuildableBeFinalized = true;
		MakeMaterialHoloColor(ActiveBuildingComponentProxy, BlueHoloMaterial);
	}

	/*FBox ActorBox = ActiveBuildingComponentProxy->GetComponentsBoundingBox();
	DrawDebugBox(GetWorld(), ActorBox.GetCenter(), ActorBox.GetExtent(), FColor::Red, false, 1.0f, 0, 5.0f);*/
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

/* Material Shading */
void UBuildSystemManagerComponent::HandleTrapMaterialChange(bool bCanTrapBeFinalized)
{
	if (bCanTrapBeFinalized)
	{
		//GEngine->AddOnScreenDebugMessage(3, 4.0, FColor::Red, "Trap Can Be Finalized. Setting Color to Blue.");
		MakeMaterialHoloColor(ActiveTrapComponentProxy, BlueHoloMaterial);
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(3, 4.0, FColor::Red, "Trap Cant Be Finalized. Setting Color to Red.");
		MakeMaterialHoloColor(ActiveTrapComponentProxy, RedHoloMaterial);
	}
}

void UBuildSystemManagerComponent::MakeMaterialHoloColor(AActor* BuildingComponentActor, UMaterial* HoloMaterialColor)
{
	//Should get the static meshes for the new Building Component everytime we try to change the color.
	ClearStoredStaticMeshes();
	
	if (StaticMeshs.Num() == 0)
	{
		GetStaticMeshComponents(BuildingComponentActor);
	}

	// Get all Static Meshes from the Actor
	if (StaticMeshs.Num() > 0)
	{
		//GEngine->AddOnScreenDebugMessage(5, 3.0f, FColor::Green, "Changing mesh color now.");
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

void UBuildSystemManagerComponent::AddToBuildableAttachments(ABuildableBase* AttachingBuildable)
{
	ATimberPlayableCharacter* PlayerCharacter = Cast<ATimberPlayableCharacter>(GetOwner());
	if (PlayerCharacter && PlayerCharacter->HoveredBuildingComponent && AttachingBuildable)
	{
		ATimberBuildingComponentBase* BuildingComponentBase = Cast<ATimberBuildingComponentBase>(PlayerCharacter->HoveredBuildingComponent);
		if (BuildingComponentBase)
		{
			BuildingComponentBase->AttachedBuildingComponents.Add(AttachingBuildable);
		}
	}
}

/* Spawn Final */
void UBuildSystemManagerComponent::SpawnFinalBuildable()
{
	if(ActiveBuildableComponentClass && BuildableRef)
	{
		// If player can afford the transaction, apply the transaction and spawn the final building component.
		if(Cast<ATimberPlayableCharacter>(GetOwner()) && Cast<ATimberPlayableCharacter>(GetOwner())
		->InventoryManager->bCanAffordCost(BuildableRef->BuildableCost))
		{
			FActorSpawnParameters SpawnParameters;
			if (ActiveBuildableComponentClass->IsChildOf(ATrapBase::StaticClass()))
			{
				SpawnFinalTrap(SpawnParameters);
				PlayBuildablePlacementSound();
			}
			else if (ActiveBuildableComponentClass->IsChildOf(ATimberBuildingComponentBase::StaticClass()))
			{
				SpawnFinalBuildingComponent(SpawnParameters);
				PlayBuildablePlacementSound();
			}
			else if (ActiveBuildableComponentClass->IsChildOf(ARampBase::StaticClass()))
			{
				SpawnFinalRampComponent(SpawnParameters);
				PlayBuildablePlacementSound();
			}
			else if (ActiveBuildableComponentClass->IsChildOf(ATeleportConstruct::StaticClass()))
			{
				SpawnTemporaryTeleportConstruct(SpawnParameters);
				PlayBuildablePlacementSound();
			}
			else if (ActiveBuildableComponentClass->IsChildOf(APowerPlate::StaticClass()))
			{
				SpawnFinalCenterSnapFloorOnlyBuildingComponent(SpawnParameters);
				PlayBuildablePlacementSound();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Player Can Not Afford this Buildable"));
			}

			//Broadcast to Tutorial Game State to Increment State.
			ADieRobotGameStateBase* DieRobotGameStateBase = Cast<ADieRobotGameStateBase>(GetWorld()->GetGameState());
			if (DieRobotGameStateBase)
			{
				ETutorialState CurrentState = DieRobotGameStateBase->TutorialState;
				if (CurrentState == ETutorialState::Building1)
				{
					TutorialBuildsPlaced += 1;
					if (TutorialBuildsPlaced == 2)
					{
						//Wave Ready to begin.
						DieRobotGameStateBase->ChangeTutorialGameState(ETutorialState::Building2);
					}
				}
			}
		}
		else
		{
			PlayBuildDeniedSound();
		}
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(4, 3.0f, FColor::Magenta, "No Active Buildable Class.");
	}
	
}

void UBuildSystemManagerComponent::SpawnFinalRampComponent(FActorSpawnParameters SpawnParameters)
{
	if (ActiveRampComponentProxy && ActiveRampComponentProxy->GetRampFinalization())
	{
		//Spawn Final Ramp at the Component Proxies location
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>
		(
			ActiveBuildableComponentClass,
			ActiveRampComponentProxy->GetActorLocation(),
			ActiveRampComponentProxy->GetActorRotation(),
			SpawnParameters);
		if(SpawnedActor)
		{
			RedrawPathTraceHandle.Broadcast();

			//Adding Ramp to Hovered Building Components Attachment Array, on Deletion or Destruction, Ramp will also be deleted.
			AddToBuildableAttachments(Cast<ABuildableBase>(SpawnedActor));
			
			Cast<ATimberPlayableCharacter>(GetOwner())->InventoryManager->bHandleBuildableTransaction(BuildableRef->BuildableCost);
		}
	}
	else
	{
		PlayBuildDeniedSound();
	}
}

void UBuildSystemManagerComponent::SpawnFinalTrap(FActorSpawnParameters SpawnParameters)
{
	if (ActiveTrapComponentProxy && ActiveTrapComponentProxy->GetCanTrapBeFinalized())
	{
		//Spawn Final Trap
		//GEngine->AddOnScreenDebugMessage(5, 3, FColor::Black, "Trap Finalized and spawned.");
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>
		(
			ActiveBuildableComponentClass,
			ActiveTrapComponentProxy->GetActorLocation(),
			ActiveTrapComponentProxy->GetActorRotation(),
			SpawnParameters);

		ATrapBase* FinalizedTrap = Cast<ATrapBase>(SpawnedActor);
		if (FinalizedTrap &&  ActiveTrapComponentProxy->TrapHoveredBuildingComponent)
		{
			//Passing the Trap Direction from the Proxy to the Final Trap.
			FinalizedTrap->BuildingComponentTrapDirection = ActiveTrapComponentProxy->BuildingComponentTrapDirection;
			
			//Setting Parent-BC-Ref on Trap.
			FinalizedTrap->ParentBuildingComponent = ActiveTrapComponentProxy->TrapHoveredBuildingComponent;

			//Resetting the Trap Direction back to default for further usage of the trap component.
			ActiveTrapComponentProxy->BuildingComponentTrapDirection = EBuildingComponentTrapDirection::Default;

			BuildableRef = Cast<ABuildableBase>(FinalizedTrap);
			
			//Uses the solved Trap Direction to know which snap points to occupy on the Building Component (Wall/Floor)
			EBuildingComponentTrapDirection TrapDirection = FinalizedTrap->BuildingComponentTrapDirection;
			switch (TrapDirection)
			{
			case EBuildingComponentTrapDirection::Front:
				ActiveTrapComponentProxy->TrapHoveredBuildingComponent->FrontTrap = FinalizedTrap;
				UE_LOG(LogTemp, Warning, TEXT("Set Trap on Building Component Front Trap Slot."));
				break;
			case EBuildingComponentTrapDirection::Back:
				ActiveTrapComponentProxy->TrapHoveredBuildingComponent->BackTrap = FinalizedTrap;
				UE_LOG(LogTemp, Warning, TEXT("Set Trap on Building Component Back Trap Slot."));
				break;
			case EBuildingComponentTrapDirection::Default:
				GEngine->AddOnScreenDebugMessage(3, 3.0f, FColor::Red, "Trap Direction Not Specified.");
				break;
			}
		}
		//Uses hovered building component from player as the Parent Building Component to attach to.
		AddToBuildableAttachments(Cast<ABuildableBase>(FinalizedTrap));
		
        //Handle Inventory Transaction if Spawn is Successfull. We check if player can afford in the SpawnFinalBuildable()
		Cast<ATimberPlayableCharacter>(GetOwner())->InventoryManager->bHandleBuildableTransaction(BuildableRef->BuildableCost);
		
		ResetBuildableComponents(ATrapBase::StaticClass());
	}
	else
	{
		PlayBuildDeniedSound();
		GEngine->AddOnScreenDebugMessage(8, 5.0f, FColor::Red, "Trap Cannot Be Finalized. BuildSystemManager-SpawnFinalBuildingComponent");
	}

}

void UBuildSystemManagerComponent::SpawnFinalBuildingComponent(FActorSpawnParameters SpawnParameters)
{
	//Use the InputTransform as the Location to Spawn the ActiveBuildingComponent
	if(ActiveBuildableComponentClass && ActiveBuildingComponentProxy && ActiveBuildingComponentProxy->bCanBuildableBeFinalized)
	{
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>
		(
		ActiveBuildableComponentClass,
		ActiveBuildingComponentProxy->GetActorLocation(),
		ActiveBuildingComponentProxy->GetActorRotation(),
		SpawnParameters
		);
		if(SpawnedActor)
		{
			SpawnedActor->SetActorEnableCollision(true);
			RedrawPathTraceHandle.Broadcast();

			Cast<ATimberPlayableCharacter>(GetOwner())->InventoryManager->bHandleBuildableTransaction(BuildableRef->BuildableCost);
		}
	}
	else if (!ActiveBuildingComponentProxy->bCanBuildableBeFinalized)
	{
		PlayBuildDeniedSound();
		//GEngine->AddOnScreenDebugMessage(5, 3.0f, FColor::Red, TEXT("Building Component Cannot Be Finalized. Overlapping other building Component."));
	}
}

void UBuildSystemManagerComponent::SpawnTemporaryTeleportConstruct(FActorSpawnParameters SpawnParameters)
{
	// Called on Input from Controller
	//Spawning Temporary Teleport Construct - This is 1/2 of the Teleport Pair
	
	if (ActiveTeleportConstructProxy)
	{
		//Spawn the Teleport half.
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
			ActiveBuildableComponentClass,
			ActiveTeleportConstructProxy->GetActorLocation(),
			ActiveTeleportConstructProxy->GetActorRotation(),
			SpawnParameters);
		
		BuildableRef = Cast<ABuildableBase>(SpawnedActor);
		ATeleportConstruct* TeleportConstruct = Cast<ATeleportConstruct>(SpawnedActor);

		//Attaching Teleporter to the Hovered Building Component, when Destroyed, Teleporter handles destruction of its own pair.
		AddToBuildableAttachments(Cast<ABuildableBase>(SpawnedActor));
		
		if (TeleportConstruct)
		{
			BuildableRef = Cast<ABuildableBase>(SpawnedActor);
			Cast<ATimberPlayableCharacter>(GetOwner())->InventoryManager->bHandleBuildableTransaction(BuildableRef->BuildableCost);

			//Handle Pairing - BSM
			//This is the BuildSystemManager's Memory of the Teleport Construct Pair
			if (TeleportTempPair.Key == nullptr)
			{
				//Makes newly Spawned Material the Yellow Holo Material - Because it is temporary. We will move this to a delegate system.
				MakeMaterialHoloColor(TeleportConstruct, YellowHoloMaterial);
				//Setting the First Half of the Teleport Construct - Stored on BSM
				TeleportTempPair.Key = TeleportConstruct;
				//This is the first half of the Teleport Construct - storing in Memory and preparing for the second half.
				TeleportConstruct->TeleportConstructState = ETeleportConstructState::Temporary;
			}
			else if (TeleportTempPair.Key && TeleportTempPair.Value == nullptr)
			{
				//Second Half of the Teleport Construct - Setting the Pairing - Stored on BSM
				TeleportTempPair.Value = TeleportConstruct;

				/*Now there is a full Pair of teleporters -> Clean up*/
				
				//Resetting Local Value to new value
				// Local Value       -     Stored on BSM
				ATeleportConstruct* TeleportConstructAlpha = TeleportTempPair.Key;
				ATeleportConstruct* TeleportConstructBeta = TeleportTempPair.Value;
				
				//Each Teleport Item now has a reference to the other in its pair.
				//The Teleport Pair is stored on the Teleport Actor Class - this is how the teleporters will reference each other.
				TeleportConstructAlpha->TeleportPair = TeleportConstructBeta;
				TeleportConstructBeta->TeleportPair = TeleportConstructAlpha;

				//Linking the Destruct Delegates for the pairs.
				TeleportConstructAlpha->LinkToPair(TeleportConstructBeta);
				TeleportConstructBeta->LinkToPair(TeleportConstructAlpha);

				//Setting Finalized State
				TeleportConstructAlpha->TeleportConstructState = ETeleportConstructState::Final;
				TeleportConstructBeta->TeleportConstructState = ETeleportConstructState::Final;

				//Applying the default materials for the Teleporter - Stored on the Teleporter after BeginPlay() 
				TeleportConstructAlpha->ApplyDefaultMaterials();
				TeleportConstructBeta->ApplyDefaultMaterials();
				
				//Clear the Temp Pair on the BSM -  We don't need the local values if the Teleport Construct has ref to each other.
				TeleportTempPair.Key = nullptr;
				TeleportTempPair.Value = nullptr;
			}
		}
		
	}
}

void UBuildSystemManagerComponent::SpawnFinalCenterSnapFloorOnlyBuildingComponent(FActorSpawnParameters SpawnParameters)
{
	//Spawn the Final Center Snap Floor Only Building Component
	//PowerPlate
	if (CenterSnapFloorOnlyBuildingComponentProxy && 
	CenterSnapFloorOnlyBuildingComponentProxy->bCanBuildableBeFinalized)
	{
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>
		(
			ActiveBuildableComponentClass,
			CenterSnapFloorOnlyBuildingComponentProxy->GetActorLocation(),
			CenterSnapFloorOnlyBuildingComponentProxy->GetActorRotation(),
			SpawnParameters);

		if (SpawnedActor)
		{
			/*
			 * Attaching the Buildable to the Building Component Slot
			 * Lets the player know that another Buildable can not be placed in this slot.
			 */
			ATimberPlayableCharacter* PlayerCharacter = Cast<ATimberPlayableCharacter>(GetOwner());
			if (PlayerCharacter && PlayerCharacter->HoveredBuildingComponent)
			{
				if (ATimberBuildingComponentBase* BuildingComponent = Cast<ATimberBuildingComponentBase>(PlayerCharacter->HoveredBuildingComponent))
				{
					BuildingComponent->FrontTrap = Cast<ABuildableBase>(SpawnedActor);
					UE_LOG(LogTemp, Warning, TEXT("Construct set on FrontTrapSnap Slot."))
					APowerPlate* PowerPlate = Cast<APowerPlate>(SpawnedActor);

					//Setting the parent floor component on this actor, so we can free up slot during deletion.
					if (PowerPlate && Cast<ATimberBuildingComponentBase>(PlayerCharacter->HoveredBuildingComponent))
					{
						PowerPlate->ParentBuildingComponent = Cast<ATimberBuildingComponentBase>(PlayerCharacter->HoveredBuildingComponent);
					}
				}
			}
			//Sets up BC to be destroyed when the Snapped to Buildable is destroyed.
			AddToBuildableAttachments(Cast<ABuildableBase>(SpawnedActor));

			Cast<ATimberPlayableCharacter>(GetOwner())->InventoryManager->bHandleBuildableTransaction(BuildableRef->BuildableCost);
		}
	}
	else
	{
		PlayBuildDeniedSound();
	    UE_LOG(LogTemp, Warning, TEXT("BSMC - SpawnFinalFloorOnlyBC - Buildable Can Not be Finalized."));
	}
}

/*Spawn Proxy*/

void UBuildSystemManagerComponent::SpawnBuildingComponentProxy(FVector SpawnVector, FRotator SpawnRotator)
{
	if (ActiveBuildableComponentClass)
	{
		const FVector Location = SnapToGrid(SpawnVector);
		const FVector Location1 = SpawnVector;
		const FRotator Rotation = SnapToRotation(SpawnRotator);
		const FActorSpawnParameters SpawnParameters;

		FTransform SpawnTransform = FTransform(Rotation, Location1, FVector(1.0f, 1.0f, 1.0f));

		//Use the InputTransform as the Location to Spawn the ActiveBuildingComponent
		//Allows Setting of Properties on actor during runtime that are needed during BeginPlay()
		ATimberBuildingComponentBase* SpawnedActor = GetWorld()->SpawnActorDeferred<ATimberBuildingComponentBase>
		(
			ActiveBuildableComponentClass,
			SpawnTransform);

		//Because proxy, handles some collision of proxy with already placed building components.
		if (SpawnedActor)
		{
			ActiveBuildingComponentProxy = SpawnedActor;
			ActiveBuildingComponentProxy->bIsProxy = true;
			UE_LOG(LogTemp, Warning, TEXT("Building Component Proxy Spawned and Set to bIsProxyTrue."));
			ActiveBuildingComponentProxy->FinishSpawning(SpawnTransform);
			DisableBuildableProxyCollisions(ActiveBuildingComponentProxy);
			MakeMaterialHoloColor(SpawnedActor, BlueHoloMaterial);

			BuildableRef = Cast<ABuildableBase>(Cast<ABuildableBase>(ActiveBuildingComponentProxy));
		}
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
		BuildableRef = Cast<ABuildableBase>(SpawnedActor);
		//Binding the Delegate Call to the newly Spawned Trap Component
		RegisterTrapComponent(ActiveTrapComponentProxy);

		//Required so that delete functionality does delete the proxy
		DisableBuildableProxyCollisions(ActiveTrapComponentProxy);
	}
}

/*Collision*/
void UBuildSystemManagerComponent::DisableBuildableProxyCollisions(ABuildableBase* BuildingComponent)
{
	BuildingComponent->SetActorEnableCollision(false);
}

/*Placement (Some With Spawning Proxy)*/
void UBuildSystemManagerComponent::HandleCenterSnapFloorOnlyPlacement(TArray<FHitResult> HitResults)
{
	//Check Hit Results for first Floor Hit and get Snap Location
	ATimberHorizontalBuildingComponent* FloorComponent = nullptr;
	FVector FloorComponentSnapLocation;
	for (FHitResult Hit : HitResults)
	{
		if (Cast<ATimberHorizontalBuildingComponent>(Hit.GetActor()))
		{
			//Getting Hit Floor Actors Center Snap Locations
			FloorComponent = Cast<ATimberHorizontalBuildingComponent>(Hit.GetActor());

			//Snapping the location to the "Upward Facing Snap Point" (Same as Traps)
			FloorComponentSnapLocation = FloorComponent->FrontTrapSnap->GetComponentLocation();
			break;
		}
	}

	//Spawn the Center Snap Floor Only Building Component Proxy if not Spawned or if the Class is different.
	if (CenterSnapFloorOnlyBuildingComponentProxy == nullptr || CenterSnapFloorOnlyBuildingComponentProxy->GetClass() != GetActiveBuildableClass())
	{
		//Spawn the first iterations of the Ramp into the world.
		//Zero Rotator ensures that the Construct is always only placed facing upward - The Actors base rotation.
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>
		(
			ActiveBuildableComponentClass,
			FloorComponentSnapLocation,
			FRotator::ZeroRotator,
			FActorSpawnParameters());
		
		CenterSnapFloorOnlyBuildingComponentProxy = Cast<ABuildableBase>(SpawnedActor);
		BuildableRef = Cast<ABuildableBase>(SpawnedActor);
		
		if (CenterSnapFloorOnlyBuildingComponentProxy)
		{
			DisableBuildableProxyCollisions(CenterSnapFloorOnlyBuildingComponentProxy);
		}
	}
	else 
	{
		//Proxy Already Spawned, handling movement of the proxy.
		BuildableRef = CenterSnapFloorOnlyBuildingComponentProxy;
		//Move the Proxy to the Center Snap of the Floor Component
		CenterSnapFloorOnlyBuildingComponentProxy->SetActorLocation(FloorComponentSnapLocation);
		MakeMaterialHoloColor(CenterSnapFloorOnlyBuildingComponentProxy, BlueHoloMaterial);
	}

	//Checking for finalization - if Building Component already has something in Snap Slot.
	if (FloorComponent && FloorComponent->FrontTrap != nullptr)
	{
		//If slot is occupied, construct can be finalized.
		CenterSnapFloorOnlyBuildingComponentProxy->bCanBuildableBeFinalized = false;
		MakeMaterialHoloColor(CenterSnapFloorOnlyBuildingComponentProxy, RedHoloMaterial);
	}
	else
	{
		CenterSnapFloorOnlyBuildingComponentProxy->bCanBuildableBeFinalized = true;
		MakeMaterialHoloColor(CenterSnapFloorOnlyBuildingComponentProxy, BlueHoloMaterial);
	}
}

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
			DisableBuildableProxyCollisions(SpawnedRamp);
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

void UBuildSystemManagerComponent::HandleTrapPlacement(TArray<FHitResult> HitResults)
{
	//Just to get Here the Raycast must have hit something.

	//SPAWNING TRAP COMPONENT
	//ATrapBase* ActiveTrapComponentProxy = GetActiveTrapComponent();
	if (ActiveTrapComponentProxy == nullptr || ActiveTrapComponentProxy->GetClass() != 
		GetActiveBuildableClass())
	{
		SpawnTrapComponentProxy(
			HitResults[0].ImpactPoint, HitResults[0].GetActor()->GetActorRotation());

		if (ActiveTrapComponentProxy)
		{
			ActiveTrapComponentProxy->SetCanTrapBeFinalized(false);
		}
	}

	// LOOKING FOR HITS ON A BUILDING COMPONENT
	// Search all hits of Multi Ray Cast for one the first that casts to a BuildingComponentBase (Ramp || Wall )
	ATimberBuildingComponentBase* FirstHitBuildingComponent = nullptr;
	for (const FHitResult& Hits : HitResults)
	{
		if (Cast<ATimberBuildingComponentBase>(Hits.GetActor()))
		{
			FirstHitBuildingComponent = Cast<ATimberBuildingComponentBase>(Hits.GetActor());
			BuildingComponentImpactPoint = Hits.ImpactPoint;
			break;
		}
	}

	//HIT A BUILDING COMPONENT - Excluding Environment Buildable
	if (FirstHitBuildingComponent && FirstHitBuildingComponent->BuildingComponentType != EBuildingComponentType::Environment && 
		FirstHitBuildingComponent->BuildableType != EBuildableType::Environment )
	{
		// Pairing the trap with the wall its Hovering over.
		if (ActiveTrapComponentProxy)
		{
		
			ActiveTrapComponentProxy->TrapHoveredBuildingComponent = FirstHitBuildingComponent;

			FTrapSnapData TrapSnapData = GetTrapSnapTransform(BuildingComponentImpactPoint, 
			FirstHitBuildingComponent, ActiveTrapComponentProxy);
			MoveBuildingComponent
			(
				FVector_NetQuantize(TrapSnapData.TrapLocation),
				ActiveTrapComponentProxy,
				TrapSnapData.TrapRotation);
		}
	}
	else // HIT BUT NOT A BUILDING COMPONENT
	{
		if (ActiveTrapComponentProxy)
		{
			ActiveTrapComponentProxy->SetCanTrapBeFinalized(false);
			ActiveTrapComponentProxy->TrapHoveredBuildingComponent = nullptr;
			if (HitResults[0].ImpactPoint != FVector::ZeroVector)
			{
				FRotator PlayerRotation = GetOwner()->GetActorTransform().GetRotation().Rotator();
				PlayerRotation.Yaw = PlayerRotation.Yaw - 180;
				MoveBuildingComponent(
					HitResults[0].ImpactPoint, ActiveTrapComponentProxy,
					PlayerRotation);
			}
		}
	}
}

// Returns the closest snap location based on the impact point of the raycast for the building component.
FTrapSnapData UBuildSystemManagerComponent::GetTrapSnapTransform(
	FVector ImpactPoint, ATimberBuildingComponentBase* BuildingComponent, ATrapBase* TrapComponentProxy)
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
				TrapComponentProxy->BuildingComponentTrapDirection = EBuildingComponentTrapDirection::Front;
				TrapComponentProxy->SetCanTrapBeFinalized(true);
			}
			else
			{
				TrapComponentProxy->SetCanTrapBeFinalized(false);
				TrapComponentProxy->BuildingComponentTrapDirection = EBuildingComponentTrapDirection::Default;
				//GEngine->AddOnScreenDebugMessage(7, 5.0f, FColor::Red, "FrontSnap Taken.");
			}
		}
		else
		{
			if (BuildingComponent->BackTrap == nullptr)
			{
				TrapSnapData.TrapLocation = BackTrapSnapLocation;
				TrapSnapData.TrapRotation = BuildingComponent->BackTrapSnap->GetComponentTransform().GetRotation().
				                                               Rotator();
				TrapComponentProxy->BuildingComponentTrapDirection = EBuildingComponentTrapDirection::Back;
				TrapComponentProxy->SetCanTrapBeFinalized(true);
			}
			else
			{
				TrapComponentProxy->SetCanTrapBeFinalized(false);
				TrapComponentProxy->BuildingComponentTrapDirection = EBuildingComponentTrapDirection::Default;
				//GEngine->AddOnScreenDebugMessage(7, 5.0f, FColor::Red, "BackSnap Taken.");
			}
		}
	}
	else
	{
		TrapComponentProxy->SetCanTrapBeFinalized(false);
		TrapComponentProxy->BuildingComponentTrapDirection = EBuildingComponentTrapDirection::Default;
	}
	return TrapSnapData;
}

void UBuildSystemManagerComponent::HandleBuildingComponentPlacement(TArray<FHitResult> HitResults)
{
	//Spawn the Proxy if it is not spawned or if the class is different.
	if (ActiveBuildingComponentProxy == nullptr || ActiveBuildingComponentProxy->GetClass() !=GetActiveBuildableClass())
	{
		SpawnBuildingComponentProxy(HitResults[0].ImpactPoint, FRotator::ZeroRotator);
	}
	
	/*Data for First Hit Building Component*/
	FHitResult BuildingComponentHitResult;
	ATimberBuildingComponentBase* FirstHitBuildingComponent = nullptr;
	
	/* Data for first Hit Building Quadrant*/
	FHitResult QuadrantHitResult;
	UBoxComponent* QuadrantHitComponent = nullptr;

	
	if(HitResults.Num() > 0)
	{
		for (FHitResult HitResult : HitResults)
		{
			if (Cast<UBoxComponent>(HitResult.GetComponent()))
			{
				//Hit Quadrant Hit
				QuadrantHitResult = HitResult;
				QuadrantHitComponent = Cast<UBoxComponent>(HitResult.GetComponent());
				if (Cast<ATimberBuildingComponentBase>(HitResult.GetActor()))
				{
					//Hit Building Component Hit
					FirstHitBuildingComponent = Cast<ATimberBuildingComponentBase>(HitResult.GetActor());
					BuildingComponentImpactPoint = HitResult.ImpactPoint;
					BuildingComponentHitResult = HitResult;

					if (FirstHitBuildingComponent && QuadrantHitComponent)
					{
						//Exiting the loop when we have what we need.
						break;
					}
				}
			}
		}
	}
	
	if(FirstHitBuildingComponent && QuadrantHitComponent)
	{
		HandleBuildingComponentSnapping(QuadrantHitResult, BuildingComponentHitResult);
	}
	else
	{
		//There is no Hit Result for either a Quadrant or a Building Component so we move it to some impact point.
		if (ActiveBuildingComponentProxy)
		{
			/*Simple Move to Location*/
			MoveBuildingComponent(HitResults[0].ImpactPoint, ActiveBuildingComponentProxy);
		}
	}
	
}

void UBuildSystemManagerComponent::HandleTeleportConstructPlacement(TArray<FHitResult> HitResults)
{
	//Check Hit Results for any Building Components that are the floor. (TimberHorizontalBuildingComponent
	ATimberHorizontalBuildingComponent* HitFloorComponent = nullptr;

	//Search Hit Results for First Floor Component
	for (FHitResult Hit : HitResults)
	{
		ATimberHorizontalBuildingComponent* TimberHorizontalBuildingComponent = Cast<ATimberHorizontalBuildingComponent>(Hit.GetActor());
		if (Hit.GetActor() == TimberHorizontalBuildingComponent)
		{
			//This is the first floor component
			HitFloorComponent = TimberHorizontalBuildingComponent;
			//UE_LOG(LogTemp, Warning, TEXT("Hit Floor Component: %s"), *HitFloorComponent->GetName());
			break;
		}
	}
	
	//Get Each Snap points location
	//This will hold the exact snaps were looking for
	TArray<USceneComponent*> FilteredSnapPoints;
	//This holds all USceneComponents
	TArray<USceneComponent*> AllSceneComponents;
	if (HitFloorComponent)
	{
		HitFloorComponent->GetComponents<USceneComponent>(AllSceneComponents);
	}else
	{
		//If we didnt hit a floor component, we dont want to continue.
		return;
	}

	//Filter Snap Points
	for (USceneComponent* SnapPoint : AllSceneComponents )
	{
		FString ComponentName = SnapPoint->GetName();
		//These are the only Snap Points I want to Snap To.
		if (ComponentName == TEXT("LeftSnap") || ComponentName == TEXT("RightSnap") || ComponentName == TEXT("TopSnap") || ComponentName == TEXT("BottomSnap"))
		{
			FilteredSnapPoints.Add(SnapPoint);
			//UE_LOG(LogTemp, Warning, TEXT("Snap Point: %s added to FilteredSnapPoints"), *SnapPoint->GetName());
		}
	}
	
	//Check distance between Raycast Hit and Snap Point
	//Get the Closest Snap Point
	float DistanceToClosestSnapPoint = 0;
	USceneComponent* ClosestSnapPoint = nullptr;
	for (USceneComponent* SnapPoint : FilteredSnapPoints)
	{
		float CheckDistance = FVector::Dist(HitResults[0].ImpactPoint, SnapPoint->GetComponentLocation());
		if (CheckDistance < DistanceToClosestSnapPoint || DistanceToClosestSnapPoint == 0)
		{
			DistanceToClosestSnapPoint = CheckDistance;
			ClosestSnapPoint = SnapPoint;
		}
	}
	
	//Spawn Proxy Teleport if none spawned.
	if (!ActiveTeleportConstructProxy && ClosestSnapPoint)
	{
		//Spawn Teleport Proxy @ Closest Snap Point
		FActorSpawnParameters SpawnParameters;
		
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>
		(
			ActiveBuildableComponentClass,
			ClosestSnapPoint->GetComponentLocation(),
			ClosestSnapPoint->GetComponentRotation(),
			SpawnParameters);
		
		if (SpawnedActor)
		{
			//Set Proxy
			ActiveTeleportConstructProxy = Cast<ATeleportConstruct>(SpawnedActor);
			//Set BuildableRef
			BuildableRef = Cast<ABuildableBase>(SpawnedActor);
			if (ActiveTeleportConstructProxy)
			{
				//Orient the Building Component to Face the Floor Center
				FVector ProxyForward = ActiveTeleportConstructProxy->GetActorLocation();
				FVector FloorCenter = HitFloorComponent->CenterSnap->GetComponentLocation();
				FRotator RotateToFace = UKismetMathLibrary::FindLookAtRotation(ProxyForward, FloorCenter) + FRotator(0, -90, 0);
				ActiveTeleportConstructProxy->SetActorRotation(RotateToFace);
				UE_LOG(LogTemp, Warning, TEXT("Teleport Construct Rotated to Face the Floor."));

				//Handle Collision
				DisableBuildableProxyCollisions(ActiveTeleportConstructProxy);
				//Set State
				ActiveTeleportConstructProxy->TeleportConstructState = ETeleportConstructState::Proxy;
				//Set Material for Proxy State
				MakeMaterialHoloColor(ActiveTeleportConstructProxy, BlueHoloMaterial); 
			}
		}
	}
	else if (ActiveTeleportConstructProxy && ClosestSnapPoint)
	{
		
		//BuildableRef = Cast<ABuildableBase>(ActiveTeleportConstructProxy);
		BuildableRef = ActiveTeleportConstructProxy;
		
		//Move the Proxy to the Closest Snap Point
		ActiveTeleportConstructProxy->SetActorLocation(ClosestSnapPoint->GetComponentLocation());
		
		//This may be off because Y is forward on the Teleport Construct
		FVector ProxyForward = ActiveTeleportConstructProxy->GetActorLocation();
		FVector FloorCenter = HitFloorComponent->CenterSnap->GetComponentLocation();
		//Set Rotation
		FRotator RotateToFace = UKismetMathLibrary::FindLookAtRotation(ProxyForward, FloorCenter) + FRotator(0, -90, 0);
		//Set Location
		ActiveTeleportConstructProxy->SetActorRotation(RotateToFace);
		
	}
}

/* Buildable Utils*/
void UBuildSystemManagerComponent::MoveBuildingComponent(
	FVector_NetQuantize Location, ABuildableBase*
	BuildingComponent, const FRotator& Rotation)
{
	if (BuildingComponent)
	{
		//OnHits we will pass in the Rotation of the HitActor so the Components Match its Rotation
		if (Rotation != FRotator::ZeroRotator)
		{
			//GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, "Rotated Trap to match Building Component.");
			BuildingComponent->SetActorRotation(Rotation);
		}
		
		//REMEMBER to change this back in the future if you want some snapping.
		//ActiveBuildingComponent->SetActorLocation(SnapToGrid(Location));
		BuildingComponent->SetActorLocation(Location, true);
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

	if (ActiveBuildableClass->IsChildOf(ATeleportConstruct::StaticClass()))
	{
		if (ActiveTeleportConstructProxy)
		{
			ActiveTeleportConstructProxy->Destroy();
			ActiveTeleportConstructProxy = nullptr;
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

	/*
	 * Why is this one so different?
	 * All of these are hardcoded per trap/buildable
	 * We basically later want to change these class calls to a type on the buildable class.
	 * The type will be closer to "CenterSnapFloorOnly" which will better describe the placement conditions.
	 * It will require a full rewrite, but basically the powerplate's placement will run on the CenterSnapFloorOnly conditions.
	 * And will just keep its reference in BuildableProxy
	 */
	if (ActiveBuildableClass->IsChildOf(APowerPlate::StaticClass()))
	{
		if (CenterSnapFloorOnlyBuildingComponentProxy)
		{
			CenterSnapFloorOnlyBuildingComponentProxy->Destroy();
			CenterSnapFloorOnlyBuildingComponentProxy = nullptr;
		}
	}

	BuildableRef = nullptr;
	
	StaticMeshs.Empty();
}

void UBuildSystemManagerComponent::RemoveBuildingComponentProxies_All()
{
	ResetBuildableComponents(ATrapBase::StaticClass());
	ResetBuildableComponents(ATimberBuildingComponentBase::StaticClass());
	ResetBuildableComponents(ARampBase::StaticClass());
	ResetBuildableComponents(ATeleportConstruct::StaticClass());
	ResetBuildableComponents(APowerPlate::StaticClass());
	
	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(3, 5.0f, FColor::Green, "ATimberPlayableCharacter::ExitBuildMode() : Building Component Proxy Removed. ");
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

void UBuildSystemManagerComponent::GetStaticMeshComponents(AActor* BuildingComponentActor)
{
	if (BuildingComponentActor)
	{
		//Gets static Meshes and returns them to the GlobalArray.
		BuildingComponentActor->GetComponents<UStaticMeshComponent>(StaticMeshs);
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
		//GEngine->AddOnScreenDebugMessage(1, 3.0, FColor::Green, "Active Building Component Class Set.");
	}
}

/*SFX*/
void UBuildSystemManagerComponent::PlayBuildablePlacementSound()
{
	USFXManagerSubsystem* SFXManager = GetWorld()->GetGameInstance()->GetSubsystem<USFXManagerSubsystem>();
	if (SFXManager)
	{
		SFXManager->OnBuildablePlacement.Broadcast();
	}
}

void UBuildSystemManagerComponent::PlayBuildDeniedSound()
{
	USFXManagerSubsystem* SFXManager = GetWorld()->GetGameInstance()->GetSubsystem<USFXManagerSubsystem>();
	if (SFXManager)
	{
		SFXManager->PlaySound("BuildDenied_1");
	}
}
