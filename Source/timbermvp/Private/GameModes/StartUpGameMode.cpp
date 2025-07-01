// Property of Paracosm Industries. Dont use my shit.


#include "GameModes/StartUpGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Subsystems/GameConfig/DieRobotGameConfigSubsystem.h"
#include "Subsystems/Music/UMusicManagerSubsystem.h"
#include "Subsystems/SaveLoad/SaveLoadSubsystem.h"
#include "Subsystems/Wave/WaveGameInstanceSubsystem.h"

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
		MusicManager->PlayMusic("CyberpunkBoy", 0.1f);
		
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
	//Called from the PlayDemo Button in the Startup Menu - See Startup Widget
	if (StartUpMenu)
	{
		StartUpMenu->RemoveFromParent();
		StartUpMenu = nullptr;

		//Runs Tutorial Etc. Standard GamePlay
		SetGameConfig(EDieRobotGameConfigType::Standard);
		
		UE_LOG(LogTemp, Warning, TEXT("Startup Game Mode - Set Game Config to Standard"));

		UGameplayStatics::OpenLevel(GetWorld(), FName("TheLab"));
		
	}
}

void AStartUpGameMode::SwitchToMidgameDemo()
{
	if (StartUpMenu)
	{
		StartUpMenu->RemoveFromParent();
		StartUpMenu = nullptr;

		//Should load from a preconfigured Save Slot.

		UWaveGameInstanceSubsystem* WaveSubsystem  = GetGameInstance()->GetSubsystem<UWaveGameInstanceSubsystem>();
		if (WaveSubsystem)
		{
			WaveSubsystem->CurrentWaveNumber = 9;
		}
		
		SetGameConfig(EDieRobotGameConfigType::MidGameDemo);
		UE_LOG(LogTemp, Warning, TEXT("Startup Game Mode - Set Game Config to MidGameDemo"));

		
		UGameplayStatics::OpenLevel(GetWorld(), FName("TheLab"));
	}
}

