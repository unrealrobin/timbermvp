// Property of Paracosm Industries. Dont use my shit.


#include "GameModes/StartUpGameMode.h"

#include "Data/MusicLibraryDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "States/DieRobotGameStateBase.h"
#include "Subsystems/GameConfig/DieRobotGameConfigSubsystem.h"
#include "Subsystems/Music/UMusicManagerSubsystem.h"

void AStartUpGameMode::BeginPlay()
{
	Super::BeginPlay();

	UDieRobotGameConfigSubsystem* DieRobotGameConfig = GetGameInstance()->GetSubsystem<UDieRobotGameConfigSubsystem>();
	if (DieRobotGameConfig)
	{
		DieRobotGameConfig->GameConfig = EDieRobotGameConfigType::MainMenu;
	}

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

void AStartUpGameMode::SetGameConfig(EDieRobotGameConfigType InGameState)
{
	UDieRobotGameConfigSubsystem* DieRobotGameConfig = GetGameInstance()->GetSubsystem<UDieRobotGameConfigSubsystem>();
	
	if (DieRobotGameConfig)
	{
		DieRobotGameConfig->GameConfig = InGameState;
	}
}

void AStartUpGameMode::SwitchToGameLevel()
{
	if (StartUpMenu)
	{
		StartUpMenu->RemoveFromParent();
		StartUpMenu = nullptr;

		//Runs Tutorial Etc. Standard GamePlay
		SetGameConfig(EDieRobotGameConfigType::Standard);

		UGameplayStatics::OpenLevel(GetWorld(), FName("TheLab"));
	}
}

void AStartUpGameMode::SwitchToMidgameDemo()
{
	if (StartUpMenu)
	{
		StartUpMenu->RemoveFromParent();
		StartUpMenu = nullptr;

		SetGameConfig(EDieRobotGameConfigType::MidGameDemo);
		
		UGameplayStatics::OpenLevel(GetWorld(), FName("TheLab"));
	}
}

