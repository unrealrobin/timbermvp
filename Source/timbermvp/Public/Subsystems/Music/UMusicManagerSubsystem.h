// Property of Paracosm Industries. 

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UMusicManagerSubsystem.generated.h"

class UMetaSoundSource;
class UMusicLibraryDataAsset;
/**
 * 
 */
UCLASS()
class TIMBERMVP_API UUMusicManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	void Initialize(FSubsystemCollectionBase& Collection) override;

	UPROPERTY()
	UMusicLibraryDataAsset* MusicLibrary;

	UPROPERTY()
	UAudioComponent* MusicPlayer;

	UPROPERTY()
	UMetaSoundSource* CurrentTrack;

	UFUNCTION()
	void PlayMusic(FName TrackName, float FadeTime=2.0f);
	
	UFUNCTION()
	void CrossfadeMusic(UMetaSoundSource* NewTrack, float FadeTime=2.0f);
};
