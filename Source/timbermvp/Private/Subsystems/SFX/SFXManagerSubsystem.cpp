// Property of Paracosm Industries. Dont use my shit.


#include "Subsystems/SFX/SFXManagerSubsystem.h"

#include "MetasoundSource.h"
#include "Components/AudioComponent.h"
#include "Data/MusicLibraryDataAsset.h"
#include "Data/MusicTrackDataAsset.h"


void USFXManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	/*Bind to all the delgates that Play Sounds - Call PlaySound with Sound*/
	OnLootPickUp.AddDynamic(this, &USFXManagerSubsystem::PlayLootPickUpSound);
	UE_LOG(LogTemp, Warning, TEXT("SFXManager Initialized."));

	//Delaying this function 1 tick.
	//Gets the SFX Library and stores it in memory.
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &USFXManagerSubsystem::GetSFXLibrary);
	
}

void USFXManagerSubsystem::GetSFXLibrary()
{
	//Update to correct DA
	static const FString SFXLibaryAssetPath = TEXT("/Game/Sounds/01SFX/DataAssets/Library/DA_SFX_Library");

	//Getting the Music Library Data Asset
	SFXLibrary = LoadObject<UMusicLibraryDataAsset>(nullptr, *SFXLibaryAssetPath);
	if (SFXLibrary)
	{
		UE_LOG(LogTemp, Warning, TEXT("SFXManager - SFX Library Loaded."));
	}
}

UMetaSoundSource* USFXManagerSubsystem::GetTrackName(FName TrackName)
{
	//Get the Tracks Data Asset from the SFX Library - @param TrackName
	UMusicTrackDataAsset* TrackData = *SFXLibrary->MusicTrackList.Find(TrackName);
	
	UMetaSoundSource* NewTrack = TrackData->TrackMetaSound;

	return NewTrack;
}

void USFXManagerSubsystem::PlaySound(UMetaSoundSource* MetaSound)
{
	if (!MetaSound) return;
	if (!GetWorld()) return;
	
	UAudioComponent* AudioComponent = NewObject<UAudioComponent>(this);
	AudioComponent->RegisterComponentWithWorld(GetWorld());
	AudioComponent->SetSound(MetaSound);

	//TODO::Add some way of cleaning up this AudioComponent reference after each time we finish the audio.
	
	AudioComponent->Play();
}

void USFXManagerSubsystem::PlayLootPickUpSound()
{
	//Will need a track Name to play the sound
	FName Name = "LootPickUp_1";
	UMetaSoundSource* MetaSound = GetTrackName(Name);
	if(MetaSound)
	{
		PlaySound(MetaSound);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Track %s not found in SFX Library"), *Name.ToString());
	}
}

void USFXManagerSubsystem::PlayWaveStartSound()
{
}

void USFXManagerSubsystem::PlayWaveEndSound()
{
}

void USFXManagerSubsystem::PlayBossSpawnSound()
{
}

void USFXManagerSubsystem::PlayBuildablePlacementSound()
{
}

void USFXManagerSubsystem::PlayUISuccessSound()
{
}

void USFXManagerSubsystem::PlayUIFailureSound()
{
}

