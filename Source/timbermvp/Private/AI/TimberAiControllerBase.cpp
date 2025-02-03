// Property of Paracosm Industries. Dont use my shit.


#include "AI/TimberAiControllerBase.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"


ATimberAiControllerBase::ATimberAiControllerBase()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTree Component");
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>("Blackboard Component");

}

void ATimberAiControllerBase::BeginPlay()
{
	Super::BeginPlay();
	
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
