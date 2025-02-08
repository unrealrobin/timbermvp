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

	//We must Initialize the the Music Manager in Every Game Mode so that it has the World Context to play Audio
	UUMusicManagerSubsystem* MusicManager = GetWorld()->GetGameInstance()->GetSubsystem<UUMusicManagerSubsystem>();
	if (MusicManager)
	{
		MusicManager->HandleInitialization();
		PlayStartUpMusic(MusicManager);
		UE_LOG(LogTemp, Warning, TEXT("Startup Level Initialized the Music Manager and Played Song."))
	}
	
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

void AStartUpGameMode::PlayStartUpMusic(UUMusicManagerSubsystem* MusicManager)
{
	//Getting the Music Manager Subsystem

	if (MusicManager)
	{
		//Plays Track with Key-value Startup1 from the TMap in MusicLibraryDataAsset
		MusicManager->PlayMusic("Startup1");
		UE_LOG(LogTemp, Warning, TEXT("Startup Music Played"));
	}
}
