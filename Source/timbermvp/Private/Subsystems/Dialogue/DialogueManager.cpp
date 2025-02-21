// Property of Paracosm Industries. Dont use my shit.


#include "Subsystems/Dialogue/DialogueManager.h"

#include "MetasoundSource.h"
#include "Components/AudioComponent.h"
#include "Data/DialogueSingle.h"
#include "Data/NarrativeDialogueLibrary.h"
#include "States/DieRobotGameStateBase.h"


void UDialogueManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UDialogueManager::LoadNarrativeDialogueLibrary);
	}
}

void UDialogueManager::LoadNarrativeDialogueLibrary()
{
	//Get the Dialogue Narrative Library
	static const FString NarrativeDialogueLibraryAssetPath = TEXT("/Game/Dialogue/DataAssetLibrary/DA_NarrativeDialogueLibrary");
	NarrativeDialogueLibrary = LoadObject<UNarrativeDialogueLibrary>(nullptr, *NarrativeDialogueLibraryAssetPath);
	if (NarrativeDialogueLibrary)
	{
		UE_LOG(LogTemp, Warning, TEXT("Dialogue Manager Subsystem - Loaded the Narrative Dialogue Library."))
	}
}

UMetaSoundSource* UDialogueManager::GetDialogueVoiceover(FName DialogueName)
{
	if (NarrativeDialogueLibrary)
	{
		UDialogueSingle* DialogueData = *NarrativeDialogueLibrary->NarrativeDialogueList.Find(DialogueName);
		if (DialogueData)
		{
			return DialogueData->DialogueVoiceOver;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Dialogue Manager Subsystem - GetDialogueVoiceover() -  Dialogue Voiceover Not Found: %s"), *DialogueName.ToString());
	return nullptr;
}

void UDialogueManager::PlayVoiceover(FName VoiceoverName)
{

	UMetaSoundSource* VoiceoverSound = GetDialogueVoiceover(VoiceoverName);
	
	if (!DialoguePlayer)
	{
		DialoguePlayer = NewObject<UAudioComponent>(this);
		DialoguePlayer->RegisterComponentWithWorld(GetWorld());
	}

	if (DialoguePlayer)
	{
		DialoguePlayer->Stop();
		DialoguePlayer->SetSound(VoiceoverSound);
		DialoguePlayer->Play();
	UE_LOG(LogTemp, Warning, TEXT("Dialogue Manager Subsystem - Playing Voiceover: %s"), *VoiceoverName.ToString());
	}

	HandlePlayedDialogue(VoiceoverName);
}

void UDialogueManager::HandlePlayedDialogue(FName VoiceoverName)
{
	if (VoiceoverName == FName("Molly_Wake_1"))
	{
		DialoguePlayer->OnAudioFinished.AddDynamic(this, &UDialogueManager::HandleWake1Finish);
	}else if (VoiceoverName == FName("Molly_Wake_2"))
	{
		DialoguePlayer->OnAudioFinished.RemoveDynamic(this, &UDialogueManager::HandleWake1Finish);
	}
	
}


/*Tutorial Dialogue Handling*/
void UDialogueManager::HandleWake1Finish()
{
	ADieRobotGameStateBase* DieRobotGameState = GetWorld()->GetGameState<ADieRobotGameStateBase>();
	if (DieRobotGameState)
	{
		//State Change initiates Broadcast to Listeners
		DieRobotGameState->ChangeTutorialGameState(ETutorialState::Wake2);

		//Playing the Next Voiceover
		PlayVoiceover(FName("Molly_Wake_2"));
	}
}

