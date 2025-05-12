// Property of Paracosm Industries.


#include "AI/Behavior/Tasks/Task_SetClosestWall.h"

#include "AI/TimberAiControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BuildSystem/BuildingComponents/TimberHorizontalBuildingComponent.h"
#include "BuildSystem/buildingComponents/TimberVerticalBuildingComponent.h"
#include "BuildSystem/Ramps/RampBase.h"
#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

UTask_SetClosestWall::UTask_SetClosestWall()
{
	NodeName = "AiSetClosestWall";
}

EBTNodeResult::Type UTask_SetClosestWall::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ATimberAiControllerBase* AIControllerBase = Cast<ATimberAiControllerBase>(OwnerComp.GetAIOwner());
	if (!AIControllerBase) return EBTNodeResult::Failed;

	ATimberEnemyCharacter* AiEnemyCharacter = Cast<ATimberEnemyCharacter>(AIControllerBase->GetPawn());
	if (!AiEnemyCharacter) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent) return EBTNodeResult::Failed;

	TArray<AActor*> WallsOrRampsArray;

	//Gets all vertical building components within RadiusToCheck (Set on the Task in the Editor)
	bool bHits = UKismetSystemLibrary::SphereOverlapActors(GetWorld(),
		AiEnemyCharacter->GetActorLocation(),
		RadiusToCheck,
		TArray<TEnumAsByte<EObjectTypeQuery>>(),
		ABuildableBase::StaticClass(),
		TArray<AActor*>(),
		WallsOrRampsArray);

	if (bHits)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Vertical Building Components."))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Matching Building Components Found."));
	}
	
	//Finding the Closest Wall
	for (AActor* Hit : WallsOrRampsArray)
	{
		if (ABuildableBase* HitBuildable = Cast<ABuildableBase>(Hit))
		{
			if (HitBuildable->BuildableType != EBuildableType::Environment)
			{
				//If this hit Returns A Wall Base Class or a Ramp Base Class
				if (Cast<ATimberVerticalBuildingComponent>(HitBuildable) || Cast<ARampBase>(HitBuildable))
				{
					float Distance = FVector::Dist(AiEnemyCharacter->GetActorLocation(), HitBuildable->GetActorLocation());
					if (Distance < MatchStruct.Distance)
					{
						MatchStruct.WallorRamp = HitBuildable;
						MatchStruct.Distance = Distance;
					}
				}
			}
		}
	}

	//Setting the Closest WallorRamp on the Blackboard
	BlackboardComponent->SetValueAsObject(FName("ClosestBuildingComponentActor"), Cast<ABuildableBase>(MatchStruct.WallorRamp));

	if (bShowDebugSphere)
	{
		if (MatchStruct.WallorRamp)
		{
			DrawDebugSphere(GetWorld(), MatchStruct.WallorRamp->GetActorLocation(), 10.0f, 12, FColor::Red, false, 5.0f);
		}
	}

	MatchStruct.WallorRamp = nullptr;
	MatchStruct.Distance = UE_MAX_FLT;
	
	return EBTNodeResult::Succeeded;
}
