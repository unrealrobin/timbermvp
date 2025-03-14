// Property of Paracosm Industries. Dont use my shit.


#include "GameModes/StartUpGameMode.h"

#include "Data/MusicLibraryDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "States/DieRobotGameStateBase.h"
#include "Subsystems/Music/UMusicManagerSubsystem.h"

void AStartUpGameMode::BeginPlay()
{
	Super::BeginPlay();

	ADieRobotGameStateBase* DieRobotGameState = Cast<ADieRobotGameStateBase>(GetWorld()->GetGameState());
	if (DieRobotGameState)
	{
		DieRobotGameState->CurrentGameState = EGameState::MainMenu;
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

void AStartUpGameMode::SetGameState(EGameState InGameState)
{
	ADieRobotGameStateBase* DieRobotGameState = Cast<ADieRobotGameStateBase>(GetWorld()->GetGameState());
	if (DieRobotGameState)
	{
		DieRobotGameState->CurrentGameState = InGameState;
	}
}

void AStartUpGameMode::SwitchToGameLevel()
{
	if (StartUpMenu)
	{
		StartUpMenu->RemoveFromParent();
		StartUpMenu = nullptr;

		//Runs Tutorial Etc. Standard GamePlay
		SetGameState(EGameState::Standard);
		UE_LOG(LogTemp, Warning, TEXT("Startup Game Mode - Setting Game State."))
		UGameplayStatics::OpenLevel(GetWorld(), FName("TheLab"));
	}
}

void AStartUpGameMode::SwitchToMidgameDemo()
{
	if (StartUpMenu)
	{
		StartUpMenu->RemoveFromParent();
		StartUpMenu = nullptr;

		UE_LOG(LogTemp, Warning, TEXT("Startup Game Mode - Setting Game State."))
		SetGameState(EGameState::MidGameDemo);

		//Used for HUD Initialization - Lets HUD now to show all the widgets and not play them gradually like in the Tutorial.
		ADieRobotGameStateBase* DieRobotGameStateBase = Cast<ADieRobotGameStateBase>(GetWorld()->GetGameState());
		if (DieRobotGameStateBase)
		{
			DieRobotGameStateBase->TutorialState = ETutorialState::TutorialComplete;
		}
		
		UGameplayStatics::OpenLevel(GetWorld(), FName("TheLab"));
	}
}

