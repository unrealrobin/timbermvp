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

	/* AI Setup */

	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBlackboardData* BlackboardDataAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ai Behavior")
	UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBlackboardComponent* BlackboardComponent;
	
};
