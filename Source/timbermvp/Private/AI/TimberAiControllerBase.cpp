// Property of Paracosm Industries.


#include "AI/TimberAiControllerBase.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/Navigation/NavigationHelperComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


ATimberAiControllerBase::ATimberAiControllerBase()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTree Component");
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>("Blackboard Component");

}

void ATimberAiControllerBase::BeginPlay()
{
	Super::BeginPlay();

	
	GetWorld()->GetTimerManager().SetTimer(CheckOnNavMeshTimerHandle, this, &ATimberAiControllerBase::CheckIsOwnerOnNavMesh, 0.5f, true );
	OwningCharacter = Cast<ATimberEnemyCharacter>(GetPawn());
	if (!OwningCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Ai Controller could not get Owning Character!"));
	}
}

void ATimberAiControllerBase::CheckIsOwnerOnNavMesh()
{
	
	if (OwningCharacter)
	{
		UNavigationSystemV1* NavSys =
			FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

		static const FVector ProbeExtent(34.f, 34.f, 200.f);   // tweak to match agent

		FNavLocation Dummy;

		if (NavSys)
		{
			//Checking if Current Location is on Nav mesh
			bool bOnNavMesh = NavSys->ProjectPointToNavigation(OwningCharacter->GetActorLocation(), Dummy,ProbeExtent);

			//No?
			if (!bOnNavMesh)
			{
				UE_LOG(LogTemp, Warning, TEXT("Pawn is NOT on NavMesh!"));

				ATimberEnemyCharacter* TimberEnemyCharacter = Cast<ATimberEnemyCharacter>(GetPawn());
				if (TimberEnemyCharacter && TimberEnemyCharacter->GetCharacterMovement())
				{
					//Checking if the character is on the Ground
					if (!TimberEnemyCharacter->GetCharacterMovement()->IsFalling())
					{
						UE_LOG(LogTemp, Warning, TEXT("Moving Pawn to NavMesh"));
						HandleActorOffMesh(TimberEnemyCharacter);
					}
				}
			}
			else
			{
				// Pawn is on navmesh
				// Do something here if needed
			}
			//bIsOwnerOnNavMesh = bOnNavMesh;
		}
	}
}

void ATimberAiControllerBase::HandleActorOffMesh(ATimberEnemyCharacter* EnemyCharacter)
{
	//Logic to handle when the pawn is not on the navmesh
	//Stop Timer
	GetWorld()->GetTimerManager().PauseTimer(CheckOnNavMeshTimerHandle);
	//Stop AI Brain
	if (BrainComponent)
	{
		BrainComponent->PauseLogic("Off Nav Mesh");
	}
	//Get Closest point on Mesh
	UNavigationSystemV1* NavSys =FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSys)
	{
		//This is the closest point found by the ProjectPointToNavigation function.
		FNavLocation ClosestPoint;
		//This is the Node Reference of the closest point found by the ProjectPointToNavigation function.
		NavNodeRef Node = ClosestPoint.NodeRef;
		//This is the size space we are checking for the closest point on the Nav Mesh;
		FVector SearchExtent = FVector(300, 300, 100);
		
		bool FoundLocation = NavSys->ProjectPointToNavigation(GetPawn()->GetActorLocation(), ClosestPoint, SearchExtent);
		
		FVector NodeCenter = FVector::ZeroVector;
		if (FoundLocation && ClosestPoint.Location != FVector::ZeroVector)
		{
			//Node ID on the Recast Nav Mesh
			if (OwningCharacter && OwningCharacter->NavHelperComponent)
			{
				NodeCenter = OwningCharacter->NavHelperComponent->GetCenterOfNode(Node);
				NodeCenter.Z += EnemyCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
				GetPawn()->SetActorLocation(NodeCenter);
				UE_LOG(LogTemp, Warning, TEXT("Moved off mesh actor to closest point on mesh"));
			}
		}
		else
		{
			//Detonate Actor causing Damage
			GetPawn()->Destroy();
			UE_LOG(LogTemp, Warning, TEXT("No Closest Point on NavMesh. Detonating Actor!"));
		}
		
	}
	//Confirm Actor is on Mesh
	CheckIsOwnerOnNavMesh();
	if (bIsOwnerOnNavMesh)
	{
		//Restart Timer
		GetWorld()->GetTimerManager().UnPauseTimer(CheckOnNavMeshTimerHandle);
		//Start AI Brain
		if (BrainComponent)
		{
			BrainComponent->ResumeLogic("On Nav Mesh");
		}
	}
}


void ATimberAiControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (BehaviorTreeAsset)
	{
		if (UseBlackboard(BlackboardDataAsset, BlackboardComponent))
		{
			RunBehaviorTree(BehaviorTreeAsset);
		}
	}
}
