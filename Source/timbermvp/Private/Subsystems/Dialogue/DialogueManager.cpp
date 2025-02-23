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

void UDialogueManager::BindToGameState()
{
	ADieRobotGameStateBase* DieRobotGameState =  Cast<ADieRobotGameStateBase>(GetWorld()->GetGameState());
	if (DieRobotGameState)
	{
		DieRobotGameState->OnTutorialStateChange.AddDynamic(this, &UDialogueManager::HandleTutorialStateChanges);
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
	}else if (VoiceoverName == FName("Molly_Wake_3"))
	{
		DialoguePlayer->OnAudioFinished.AddDynamic(this, &UDialogueManager::HandleWake3Finish);
	}
	
}

void UDialogueManager::HandleTutorialStateChanges(ETutorialState NewState)
{
	switch (NewState)
	{
	case ETutorialState::Wake1:
		break;
	case ETutorialState::Wake2:
		break;
	case ETutorialState::Wake3:
		break;
	case ETutorialState::Combat1:
		break;
	case ETutorialState::Combat2:
		//PlayVoiceover("Molly_Combat_Quip_1");
		break;
	case ETutorialState::Parts1:
		PlayVoiceover("Molly_Parts_1");
		break;
	case ETutorialState::Building1:
		break;
	case ETutorialState::Building2:
		break;
	case ETutorialState::Building3:
		break;
	case ETutorialState::WaveStart:
		break;
	case ETutorialState::WaveComplete:
		break;
	case ETutorialState::Default:
		break;
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

void UDialogueManager::HandleWake3Finish()
{
	ADieRobotGameStateBase* DieRobotGameState = GetWorld()->GetGameState<ADieRobotGameStateBase>();
	if (DieRobotGameState)
	{
		//State Change initiates Broadcast to Listeners
		DieRobotGameState->ChangeTutorialGameState(ETutorialState::Combat1);

		//Initiating Combat Training
		PlayVoiceover("Molly_Combat_1");

		//Removing the Binding.
		DialoguePlayer->OnAudioFinished.RemoveDynamic(this, &UDialogueManager::HandleWake3Finish);
	}
}

