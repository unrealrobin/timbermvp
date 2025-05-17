// Property of Paracosm Industries.


#include "AI/TimberAiControllerBase.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Components/CapsuleComponent.h"
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
	
}

void ATimberAiControllerBase::CheckIsOwnerOnNavMesh()
{
	
	if (ATimberEnemyCharacter* OwningCharacter = Cast<ATimberEnemyCharacter>(GetPawn()))
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
		FNavLocation ClosestPoint;
		FVector SearchExtent = FVector(300, 300, 100);
		bool FoundLocation = NavSys->ProjectPointToNavigation(GetPawn()->GetActorLocation(), ClosestPoint, SearchExtent);
		if (FoundLocation)
		{
			//Move Actor to "On Mesh"
			//Controller makes the Actor move to location
			//Adjusting Location because Root Component on Actor is at Abdomen so we need to offset the Height Approx .5 Capsule Component Height.
			FVector Location = ClosestPoint.Location;
			Location.Z += EnemyCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
			GetPawn()->SetActorLocation(Location);
			UE_LOG(LogTemp, Warning, TEXT("Moved off mesh actor to closest point on mesh"));
		}
		else
		{
			//Detonate Actor causing Damage
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
