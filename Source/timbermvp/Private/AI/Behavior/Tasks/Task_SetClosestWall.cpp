// Property of Paracosm Industries. Dont use my shit.


#include "AI/Behavior/Tasks/Task_SetClosestWall.h"

#include "AI/TimberAiControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BuildSystem/BuildingComponents/TimberHorizontalBuildingComponent.h"
#include "BuildSystem/buildingComponents/TimberVerticalBuildingComponent.h"
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

	TArray<AActor*> WallsArray;

	bool bHits = UKismetSystemLibrary::SphereOverlapActors(GetWorld(),
		AiEnemyCharacter->GetActorLocation(),
		RadiusToCheck,
		TArray<TEnumAsByte<EObjectTypeQuery>>(),
		ATimberVerticalBuildingComponent::StaticClass(),
		TArray<AActor*>(),
		WallsArray);

	if (bHits)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Vertical Building Components."))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Matching Building Components Found."));
	}
	

	for (AActor* Wall : WallsArray)
	{
		if (Cast<ATimberVerticalBuildingComponent>(Wall) && Cast<ATimberVerticalBuildingComponent>(Wall)->BuildingComponentType != EBuildingComponentType::Environment)
		{
			float Distance = FVector::Dist(AiEnemyCharacter->GetActorLocation(), Wall->GetActorLocation());
			if (Distance < ClosestWall.Distance)
			{
				ClosestWall.Wall = Wall;
				ClosestWall.Distance = Distance;
			}
		}
	}

	BlackboardComponent->SetValueAsObject(FName("ClosestBuildingComponentActor"), Cast<ABuildableBase>(ClosestWall.Wall));

	if (bShowDebugSphere)
	{
		if (ClosestWall.Wall)
		{
			DrawDebugSphere(GetWorld(), ClosestWall.Wall->GetActorLocation(), 10.0f, 12, FColor::Red, false, 5.0f);
		}
	}

	ClosestWall.Wall = nullptr;
	ClosestWall.Distance = 20000.0f;
	
	return EBTNodeResult::Succeeded;
}
