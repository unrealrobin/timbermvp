// Property of Paracosm Industries. Dont use my shit.


#include "Components/Tutorial/TutorialManager.h"
#include "DialoguePlugin/Public/Dialogue.h"

// Sets default values for this component's properties
UTutorialManager::UTutorialManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TutorialEventTriggered.AddDynamic(this, &UTutorialManager::HandleTutorialEventTrigger);
	
}


// Called when the game starts
void UTutorialManager::BeginPlay()
{
	Super::BeginPlay();

	//Path to Asset
	FString DialogueAssetPath = TEXT("/Game/Dialogue/Tutorial/DA_Tutorial_1");
	TutorialDialogue = LoadObject<UDialogue>(this, *DialogueAssetPath);
	if (TutorialDialogue)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tutorial Manager - Dialogue Loaded to Tutorial Manager."))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Tutorial Manager - Dialogue not found at Asset Path or Dialogue failed to load."))
	}
	
}

void UTutorialManager::HandleTutorialEventTrigger(FName TutorialEvent)
{
	if (TutorialEvent == "WakeUp")
	{
		WakeUp();
	}
	
}

