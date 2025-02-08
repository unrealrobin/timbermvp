// Property of Paracosm Industries. Dont use my shit.


#include "GameModes/StartUpGameMode.h"

#include "Data/MusicLibraryDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/Music/UMusicManagerSubsystem.h"

void AStartUpGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!StartUpMenu )
	{
		StartUpMenu = CreateWidget<UUserWidget>(GetWorld(), StartUpMenuClass );
		if (StartUpMenu)
		{
			StartUpMenu->AddToViewport();
		}
	}

	PlayStartUpMusic();
}

void AStartUpGameMode::SwitchToGameLevel()
{
	
	if (StartUpMenu)
	{
		StartUpMenu->RemoveFromParent();
		StartUpMenu = nullptr;

		UUMusicManagerSubsystem* MusicSubsystem = GetGameInstance()->GetSubsystem<UUMusicManagerSubsystem>();

		if (MusicSubsystem)
		{
			MusicSubsystem->PlayMusic("Build1", 2.0f);
		}

		UGameplayStatics::OpenLevel(GetWorld(), FName("TheLab"));
	}
}

void AStartUpGameMode::PlayStartUpMusic()
{
	//Getting the Music Manager Subsystem
	UUMusicManagerSubsystem* MusicSubsystem = GetGameInstance()->GetSubsystem<UUMusicManagerSubsystem>();

	if (MusicSubsystem)
	{
		//Plays Track with Keyvalue Startup1 from the TMap in MusicLibraryDataAsset
		MusicSubsystem->PlayMusic("Startup1");
		UE_LOG(LogTemp, Warning, TEXT("Startup Music Played"));
	}

	
}
