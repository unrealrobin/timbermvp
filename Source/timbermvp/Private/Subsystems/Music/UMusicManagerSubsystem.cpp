// Property of Paracosm Industries. Dont use my shit.


#include "Subsystems/Music/UMusicManagerSubsystem.h"

#include "Components/AudioComponent.h"
#include "Data/MusicLibraryDataAsset.h"
#include "Kismet/GameplayStatics.h"


void UUMusicManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UUMusicManagerSubsystem::HandleInitialization);

	BlankTrack = LoadObject<UMetaSoundSource>(nullptr, TEXT("/Game/Music/MetaSounds/MS_Blank.MS_Blank"));
	
}

void UUMusicManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();

	MusicPlayerAlpha = nullptr;
}

//Called from the first world (StartupLevel) so we can use the Create2dSound function and set to Persistant Sound.
void UUMusicManagerSubsystem::HandleInitialization()
{
	//Setting the WorldContext on the Subsystem, when new GameModes Call this function, resets the world context.
	if (!WorldContext || WorldContext != GetWorld())
	{
		WorldContext = GetWorld();
	}
	
	static const FString MusicLibaryAssetPath = TEXT("/Game/Music/DataAssets/DA_MusicLibrary");

	//Getting the Music Library Data Asset
	MusicLibrary = LoadObject<UMusicLibraryDataAsset>(nullptr, *MusicLibaryAssetPath);

	if (!MusicPlayerAlpha)
	{
		//Game/Music/MetaSounds/MS_Blank.MS_Blank'
		
		//MusicPlayer = NewObject<UAudioComponent>(this);
		MusicPlayerAlpha = UGameplayStatics::SpawnSound2D(GetWorld(),
			BlankTrack,
			1.0f,
			1.0f,
			0.0f,
			nullptr,
			true,
			false
			);
		//Because this function is called from a Game Mode the Loaded level will work.
		MusicPlayerAlpha->RegisterComponentWithWorld(WorldContext);
		MusicPlayerAlpha->bAutoActivate = true;
		MusicPlayerAlpha->bIsUISound = true;

		if (MusicPlayerAlpha)
		{
			UE_LOG(LogTemp, Warning, TEXT("Audio Object - Music Player - Created"));
		}
	}
}

UAudioComponent* UUMusicManagerSubsystem::GetUnusedAudioComponent()
{
	if (!MusicPlayerAlpha) return MusicPlayerAlpha;
	if (!MusicPlayerBeta) return MusicPlayerBeta;

	return MusicPlayerAlpha->IsPlaying() ? MusicPlayerBeta : MusicPlayerAlpha;
	
}

void UUMusicManagerSubsystem::PlayMusic(FName TrackName, float FadeTime)
{
	//Checking Initialization of the Music Player component and that a Music Library Exists.
	if (!MusicLibrary || !MusicPlayerAlpha)
	{
		UE_LOG(LogTemp, Warning, TEXT("Music Player or Music Library not found."));
		return;
	}
	//Checking that the requested track exists in the Music Library
	if (!MusicLibrary->MusicTrackList.Contains(TrackName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Track %s not found in Music Library"), *TrackName.ToString());
		return;
	}

	//Get the Tracks Data Asset from the Music Library - @param TrackName
	UMusicTrackDataAsset* TrackData = *MusicLibrary->MusicTrackList.Find(TrackName);
	
	if (TrackData)
	{
		//Stops any currently Playing Track
		MusicPlayerAlpha->Stop();
		//Setting Current track to the new track <MetaSoundSource>
		CurrentTrack = TrackData->TrackMetaSound;
		//Setting the Sound to the Music Player
		MusicPlayerAlpha->SetSound(CurrentTrack);
		//Fade In Amount for the Song / @param FadeTime / Works as Play()
		MusicPlayerAlpha->FadeIn(FadeTime);

		if (MusicPlayerAlpha->IsPlaying())
		{
			UE_LOG(LogTemp, Warning, TEXT("Now playing: %s"), *TrackName.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Not playing: %s"), *TrackName.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Track %s not found in Music Library"), *TrackName.ToString());
	}
}

void UUMusicManagerSubsystem::PlayStartUpMusic(FName TrackName)
{
	//Checking Initialization of the Music Player component and that a Music Library Exists.
	if (!MusicLibrary || !MusicPlayerAlpha)
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

	//Finding a Key Value Pair in the Music Library {TrackName, UMusicTrackDataAsset}
	UMusicTrackDataAsset* TrackData = *MusicLibrary->MusicTrackList.Find(TrackName);
	
	if (TrackData)
	{
		//Setting Current track to the new track <MetaSoundSource>
		CurrentTrack = TrackData->TrackMetaSound;
		//Setting the Sound to the Music Player
		MusicPlayerAlpha->SetSound(TrackData->TrackMetaSound);
		//Fade In Amount for the Song
		MusicPlayerAlpha->FadeIn(2.0f);
		//1.0f is Normal Volume
		MusicPlayerAlpha->SetVolumeMultiplier(1.0f);
		MusicPlayerAlpha->Play();
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Track %s not found in Music Library"), *TrackName.ToString());
	}
}

void UUMusicManagerSubsystem::CrossfadeMusic(UMetaSoundSource* NewTrack, float FadeTime)
{
	// Checking Initialization of the Music Player component and that a Music Library Exists.
	if (!MusicLibrary || !MusicPlayerAlpha)
	{
		UE_LOG(LogTemp, Warning, TEXT("Music Player or Music Library not found."));
		return;
	}

	// Fade out over the specified FadeTime
	MusicPlayerAlpha->FadeOut(FadeTime, 0.0f);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this, NewTrack, FadeTime]()
	{
		if (MusicPlayerAlpha)
		{
			// Forcing stop of the current track and resetting the state
			MusicPlayerAlpha->Stop();
			MusicPlayerAlpha->SetSound(nullptr);

			// Adding a small delay to ensure the MusicPlayer is reset
			FTimerHandle InnerTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(InnerTimerHandle, FTimerDelegate::CreateLambda([this, NewTrack, FadeTime]()
			{
				CurrentTrack = NewTrack;
				MusicPlayerAlpha->SetSound(NewTrack);

				// Ensure the MusicPlayer is ready before starting FadeIn and Play
				if (!MusicPlayerAlpha->IsPlaying())
				{
					MusicPlayerAlpha->FadeIn(FadeTime);
					//MusicPlayer->Play();
					UE_LOG(LogTemp, Warning, TEXT("Crossfading to new track"));
				}
			}), 0.1f, false);
		}
	}), FadeTime, false);
}


