// Property of Paracosm Industries. Dont use my shit.


#include "AI/TimberAiControllerBase.h"


ATimberAiControllerBase::ATimberAiControllerBase()
{
	AiBehaviorTree = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
}

void ATimberAiControllerBase::BeginPlay()
{
	Super::BeginPlay();
	AiBehaviorTree->StartLogic();
}
