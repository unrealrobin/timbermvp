// Property of Paracosm Industries. Dont use my shit.


#include "Subsystems/Music/UMusicManagerSubsystem.h"

#include "Components/AudioComponent.h"
#include "Data/MusicLibraryDataAsset.h"

void UUMusicManagerSubsystem::HandleInitialization()
{
	static const FString MusicLibaryAssetPath = TEXT("/Game/Music/DataAssets/DA_MusicLibrary");

	MusicLibrary = LoadObject<UMusicLibraryDataAsset>(nullptr, *MusicLibaryAssetPath);

	if (!MusicLibrary)
	{
		UE_LOG(LogTemp, Warning, TEXT("Music Library Data Asset not found."));
		return;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Music Library Data Asset found."));
		UE_LOG(LogTemp, Warning, TEXT("Music Library contains %d tracks"), MusicLibrary->MusicTrackList.Num());
	}

	if (!MusicPlayer)
	{

		//Finding or creating an Actor to own the Music Player
		AActor* AudioOwner = GetWorld()->GetFirstPlayerController()->GetPawn();
		if (!AudioOwner)
		{
			AudioOwner = GetWorld()->SpawnActor<AActor>();
		}

		
		//Create the Audio Component on Initialization
		MusicPlayer = NewObject<UAudioComponent>(AudioOwner);
		MusicPlayer->RegisterComponent();
		MusicPlayer->bAutoActivate = true;
		MusicPlayer->bIsUISound = true;

		if (MusicPlayer)
		{
			UE_LOG(LogTemp, Warning, TEXT("Audio Object - Music Player - Created"));
		}
	}
}

void UUMusicManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UUMusicManagerSubsystem::HandleInitialization);

	
}

void UUMusicManagerSubsystem::PlayMusic(FName TrackName, float FadeTime)
{
	//Checking Initialization of the Music Player component and that a Music Library Exists.
	if (!MusicLibrary || !MusicPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Music Player or Music Library not found."));
		return;
	}
	//Checking that the rquested track exists in the Music Library
	if (!MusicLibrary->MusicTrackList.Contains(TrackName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Track %s not found in Music Library"), *TrackName.ToString());
		return;
	}

	UMusicTrackDataAsset* TrackData = *MusicLibrary->MusicTrackList.Find(TrackName);

	
	
	if (TrackData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Track %s found in Music Library"), *TrackName.ToString());
		if (MusicPlayer->IsPlaying())
		{
			CrossfadeMusic(TrackData->TrackMetaSound, FadeTime);
		}
		else
		{
			//Setting Current track to the new track <MetaSoundSource>
			CurrentTrack = TrackData->TrackMetaSound;
			//Setting the Sound to the Music Player
			MusicPlayer->SetSound(TrackData->TrackMetaSound);
			//Fade In Amount for the Song
			MusicPlayer->FadeIn(FadeTime);

			//1.0f is Normal Volume
			MusicPlayer->SetVolumeMultiplier(1.0f);
			MusicPlayer->AdjustVolume(0.0f, 2.0f);
			
			MusicPlayer->Play();

			if (MusicPlayer->IsPlaying())
			{
				UE_LOG(LogTemp, Warning, TEXT("Now playing: %s"), *TrackName.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Not playing: %s"), *TrackName.ToString());
			}

		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Track %s not found in Music Library"), *TrackName.ToString());
	}
}


void UUMusicManagerSubsystem::CrossfadeMusic(UMetaSoundSource* NewTrack, float FadeTime)
{
	//Used to Fade out 1 song and Fade in another.
	//Checking Initialization of the Music Player component and that a Music Library Exists.
	if (!MusicLibrary || !MusicPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Music Player or Music Library not found."));
		return;
	}

	//Fades out over a Specific amount of time to the FadeVolumeLevel
	MusicPlayer->FadeOut(FadeTime, 0.0f);

	FTimerHandle TimerHandle;
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this, NewTrack,FadeTime]()
	{
		CurrentTrack = NewTrack;
		MusicPlayer->SetSound(NewTrack);
		MusicPlayer->FadeIn(FadeTime);
		MusicPlayer->Play();

		UE_LOG(LogTemp, Warning, TEXT("Crossfading to new track"));
	}), FadeTime, false);
}
