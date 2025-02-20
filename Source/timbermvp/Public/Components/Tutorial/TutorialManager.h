// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TutorialManager.generated.h"

class UDialogue;



UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class TIMBERMVP_API UTutorialManager : public UActorComponent
{
	GENERATED_BODY()


public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTutorialEventTriggered, FName, TutorialEvent);
	FOnTutorialEventTriggered TutorialEventTriggered;
	
	// Sets default values for this component's properties
	UTutorialManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	
	UFUNCTION()
	void HandleTutorialEventTrigger(FName TutorialEvent);

	UFUNCTION(BlueprintImplementableEvent)
	void WakeUp();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	UDialogue* TutorialDialogue = nullptr;
};
