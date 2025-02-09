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

	//We must Initialize the Music Manager in Every Game Mode so that it has the World Context to play Audio
	UUMusicManagerSubsystem* MusicManager = GetWorld()->GetGameInstance()->GetSubsystem<UUMusicManagerSubsystem>();
	if (MusicManager)
	{
		//AudioComponent Creation
		MusicManager->HandleInitialization();

		//Play the Startup1 Song
		MusicManager->PlayMusic("Startup1", 2.0f);
		
		UE_LOG(LogTemp, Warning, TEXT("Startup Level Initialized the Music Manager and Played Song."))
	}
	
}

void AStartUpGameMode::SwitchToGameLevel()
{
	if (StartUpMenu)
	{
		StartUpMenu->RemoveFromParent();
		StartUpMenu = nullptr;

		UGameplayStatics::OpenLevel(GetWorld(), FName("TheLab"));
	}
}

