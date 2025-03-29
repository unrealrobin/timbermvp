// Property of Paracosm Industries. Dont use my shit.


#include "AI/Behavior/Tasks/Task_AI_SetCurrentTarget.h"

#include "AI/TimberAiControllerBase.h"
#include "Character/Enemies/TimberEnemyCharacter.h"

UTask_AI_SetCurrentTarget::UTask_AI_SetCurrentTarget()
{
	NodeName = "SetActorAsCurrentTarget";
}

EBTNodeResult::Type UTask_AI_SetCurrentTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ATimberAiControllerBase* AIControllerBase = Cast<ATimberAiControllerBase>(OwnerComp.GetAIOwner());
	if (!AIControllerBase) return EBTNodeResult::Failed;

	ATimberEnemyCharacter* AiEnemyCharacter = Cast<ATimberEnemyCharacter>(AIControllerBase->GetPawn());
	if (!AiEnemyCharacter) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent) return EBTNodeResult::Failed;

	

	return EBTNodeResult::Succeeded;
}
