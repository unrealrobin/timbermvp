// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_SetClosestWall.generated.h"

USTRUCT()
struct FWallStruct
{
	GENERATED_BODY()

public:
	UPROPERTY()
	AActor* Wall = nullptr;
	UPROPERTY()
	float Distance = 20000.0f;
	
};


/**
 * 
 */
UCLASS()
class TIMBERMVP_API UTask_SetClosestWall : public UBTTaskNode
{
	GENERATED_BODY()

	UTask_SetClosestWall();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY()
	FWallStruct ClosestWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TaskData")
	float RadiusToCheck = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TaskData")
	bool bShowDebugSphere = false;
};
