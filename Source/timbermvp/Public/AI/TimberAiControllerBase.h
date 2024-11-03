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

	//write the constructor function for this class
	ATimberAiControllerBase();

public:
	void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ai Behavior")
	UBehaviorTreeComponent* AiBehaviorTree;

	
};
