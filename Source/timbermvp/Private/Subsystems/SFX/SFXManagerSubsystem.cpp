// Property of Paracosm Industries. Dont use my shit.


#include "Subsystems/SFX/SFXManagerSubsystem.h"

#include "MetasoundSource.h"
#include "Components/AudioComponent.h"
#include "Data/MusicLibraryDataAsset.h"
#include "Data/MusicTrackDataAsset.h"


void USFXManagerSubsystem::BindToSFXDelegates()
{
	/*Bind to all the delgates that Play Sounds - Call PlaySound with Sound*/
	OnLootPickUp.AddDynamic(this, &USFXManagerSubsystem::PlayLootPickUpSound);
	OnWaveStart.AddDynamic(this, &USFXManagerSubsystem::PlayWaveStartSound);
	OnWaveEnd.AddDynamic(this, &USFXManagerSubsystem::PlayWaveEndSound);
	OnBossSpawn.AddDynamic(this, &USFXManagerSubsystem::PlayBossSpawnSound);
	OnBuildablePlacement.AddDynamic(this, &USFXManagerSubsystem::PlayBuildablePlacementSound);
	OnUISuccess.AddDynamic(this, &USFXManagerSubsystem::PlayUISuccessSound);
	OnUIFailure.AddDynamic(this, &USFXManagerSubsystem::PlayUIFailureSound);
}

void USFXManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("SFXManager Initialized."));

	BindToSFXDelegates();
	
	//Gets the SFX Library and stores it in memory.
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &USFXManagerSubsystem::GetSFXLibrary);
	
}

void USFXManagerSubsystem::GetSFXLibrary()
{
	static const FString SFXLibaryAssetPath = TEXT("/Game/Sounds/01SFX/DataAssets/Library/DA_SFX_Library");
	
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
	if (!TrackData)
	{
		UE_LOG(LogTemp, Warning, TEXT("SFXManager - Track Data not found."));
	}
	
	UMetaSoundSource* NewTrack = TrackData->TrackMetaSound;

	return NewTrack;
}

void USFXManagerSubsystem::PlaySound(FName TrackName)
{
	UMetaSoundSource* MetaSound = GetTrackName(TrackName);
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
	PlaySound("LootPickUp_1");	
}

void USFXManagerSubsystem::PlayWaveStartSound()
{
	PlaySound("WaveStartAlarm_1");
}

void USFXManagerSubsystem::PlayWaveEndSound()
{
	PlaySound("WaveComplete_1");
}

void USFXManagerSubsystem::PlayBossSpawnSound()
{
	PlaySound("BossEntry_1");
}

void USFXManagerSubsystem::PlayBuildablePlacementSound()
{
	PlaySound("BuildablePlacement_1");
}

void USFXManagerSubsystem::PlayUISuccessSound()
{
	PlaySound("UI_Success_1");
}

void USFXManagerSubsystem::PlayUIFailureSound()
{
	PlaySound("UI_Failure_1");
}

