// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DialogueManager.generated.h"

class UMetaSoundSource;
class UNarrativeDialogueLibrary;
/**
 * 
 */
UCLASS()
class TIMBERMVP_API UDialogueManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	/*
	 * Dialogue Names
	 *
	 * Speaker_TutorialState_DialogueOrder#
	 * Molly_Wake_1
	 * Molly_Wake_2
	 * Molly_Wake_3
	 */
	
	void Initialize(FSubsystemCollectionBase& Collection) override;

	UPROPERTY()
	UNarrativeDialogueLibrary* NarrativeDialogueLibrary = nullptr;
	
	UPROPERTY()
	UAudioComponent* DialoguePlayer = nullptr;

	UFUNCTION()
	void LoadNarrativeDialogueLibrary();

	UFUNCTION()
	UMetaSoundSource* GetDialogueVoiceover(FName DialogueName);

	UFUNCTION(BlueprintCallable)
	void PlayVoiceover(FName VoiceoverName);

	UFUNCTION()
	void HandlePlayedDialogue(FName VoiceoverName);

	/*Tutorial Dialogue Handling*/

	UFUNCTION()
	void HandleWake1Finish();

};

