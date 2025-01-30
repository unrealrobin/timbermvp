// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "TimberAiControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class TIMBERMVP_API ATimberAiControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	ATimberAiControllerBase();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ai Behavior")
	UBehaviorTreeComponent* AiBehaviorTree;
};
