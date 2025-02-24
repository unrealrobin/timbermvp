// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DialogueManager.generated.h"

enum class ETutorialState : uint8;
class USoundMix;
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
	UAudioComponent* DialoguePlayer = nullptr;
	
	UPROPERTY()
	UNarrativeDialogueLibrary* NarrativeDialogueLibrary = nullptr;
	
	UFUNCTION()
	void LoadNarrativeDialogueLibrary();
	
	UPROPERTY()
	USoundMix* NarrativeDialogueSoundMix = nullptr;
	
	void LoadNarrativeDialogueSoundMix();

	UFUNCTION()
	void RemoveNarrativeSoundMix();


	UFUNCTION()
	UMetaSoundSource* GetDialogueVoiceover(FName DialogueName);

	UFUNCTION(BlueprintCallable)
	void PlayVoiceover(FName VoiceoverName);

	UFUNCTION()
	void HandleWake3Finish();
	UFUNCTION()
	void HandleParts1Finish();
	UFUNCTION()
	void HandleWaveStartDialogueFinish();
	UFUNCTION()
	void HandlePlayedDialogue(FName VoiceoverName);
	UFUNCTION()
	void HandleTutorialStateChanges(ETutorialState NewState);

	/*Tutorial Dialogue Handling*/
	UFUNCTION()
	void HandleWake1Finish();

	UFUNCTION()
	void BindToGameState();

};
