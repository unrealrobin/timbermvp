// Property of Paracosm.


#include "AI/Behavior/Tasks/Task_MoveThroughCorridorPath.h"

#include "AI/TimberAiControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Components/Navigation/NavigationHelperComponent.h"
#include "Navigation/PathFollowingComponent.h"

EBTNodeResult::Type UTask_MoveThroughCorridorPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	BehaviorTreeComponent = &OwnerComp;
	
	AIControllerBase = Cast<ATimberAiControllerBase>(OwnerComp.GetAIOwner());
	if (!AIControllerBase) return EBTNodeResult::Failed;

	ATimberEnemyCharacter* AiEnemyCharacter = Cast<ATimberEnemyCharacter>(AIControllerBase->GetPawn());
	if (!AiEnemyCharacter) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent) return EBTNodeResult::Failed;

	ATimberEnemyCharacter* EnemyCharacter = Cast<ATimberEnemyCharacter>(BlackboardComponent->GetValueAsObject(SelfActorKey.SelectedKeyName));
	AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetActorKey.SelectedKeyName));

	if (EnemyCharacter && EnemyCharacter->NavHelperComponent && TargetActor)
	{
		PathFollowingComponent = AIControllerBase->GetPathFollowingComponent();
		if (AIControllerBase && PathFollowingComponent)
		{
			CorridorPathPoints = EnemyCharacter->NavHelperComponent->GetCorridorPathPoints(
				EnemyCharacter->GetActorLocation(),
				TargetActor->GetActorLocation()
			);

			bIsPathPartial = EnemyCharacter->NavHelperComponent->bIsLastPathPartial;
			
			if (CorridorPathPoints.Num() > 0)
			{
				//Total Points in Corridor Path.
				TotalCorridorPoints = CorridorPathPoints.Num() - 1;
				//Callback for when the Move to the next point is finished
				PathFollowingComponent->OnRequestFinished.AddUObject(this, &UTask_MoveThroughCorridorPath::OnMoveFinished);
				//Calling Initial Move to the first point in the path.
				AIControllerBase->MoveToLocation(CorridorPathPoints[NextCorridorPoint], AcceptanceRadius);

				//Latent Task
				return EBTNodeResult::InProgress;
			}
		}
	}
	
	return EBTNodeResult::Failed;
}

void UTask_MoveThroughCorridorPath::OnMoveFinished(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (Result.Code == EPathFollowingResult::Success)
	{
		NextCorridorPoint++;
		if (NextCorridorPoint < TotalCorridorPoints)
		{
			if (AIControllerBase)
			{
				AIControllerBase->MoveToLocation(CorridorPathPoints[NextCorridorPoint], AcceptanceRadius);
				return;
			}
		}
		//NextCorridorPoint == TotalCorridorPoints (We are at the last point in the path)
		FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Succeeded );
	}
	else
	{
		FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Failed );
	}

	
}
