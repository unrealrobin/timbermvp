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

	if (BehaviorTreeAsset)
	{
		if (UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComponent))
		{
			RunBehaviorTree(BehaviorTreeAsset);
		}
	}
}
