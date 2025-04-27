// Property of Paracosm Industries. Dont use my shit.


#include "GameModes/TimberGameModeBase.h"

#include "BrainComponent.h"
#include "AI/TimberAiControllerBase.h"
#include "Character/TimberSeeda.h"
#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Components/BuildSystem/BuildSystemManagerComponent.h"
#include "Components/Inventory/InventoryManagerComponent.h"
#include "Controller/TimberPlayerController.h"
#include "Environment/DynamicLab.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Environment/LabDoorBase.h"
#include "Environment/LocationMarkerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/SaveLoad/SaveLoadStruct.h"
#include "Subsystems/Dialogue/DialogueManager.h"
#include "Subsystems/GameConfig/DieRobotGameConfigSubsystem.h"
#include "Subsystems/Music/UMusicManagerSubsystem.h"
#include "Subsystems/SaveLoad/SaveLoadSubsystem.h"
#include "Subsystems/Wave/WaveGameInstanceSubsystem.h"


class UDialogueManager;
class UBuildingComponentPanel;



void ATimberGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	InitializeGameState();

	PlayBuildMusic();

	{//Binding to Delegates
		GetWaveGameInstanceSubsystem()->OpenLabDoorHandle.AddDynamic(this, &ATimberGameModeBase::OpenLabDoors);
		GetWaveGameInstanceSubsystem()->CloseLabDoorHandle.AddDynamic(this, &ATimberGameModeBase::CloseLabDoors);
		//GetWaveGameInstanceSubsystem()->SaveCurrentGameHandle.AddDynamic(this, &ATimberGameModeBase::SaveCurrentGame);
		GetWaveGameInstanceSubsystem()->HandleWaveComplete.AddDynamic(this, &ATimberGameModeBase::HandleWaveComplete);
		/*Subscribing to Player Death Delegate Signature*/
	}
	
	GetWaveGameInstanceSubsystem()->PrepareSpawnPoints();

	{
		UDialogueManager* DialogueManager = GetWorld()->GetGameInstance()->GetSubsystem<UDialogueManager>();
		if (DialogueManager)
		{
			DialogueManager->BindToGameState();
		}
	}
	
	GatherSeedaData();
	GatherAllLabDoors();

	if(WaveCompositionDataTable)
	{
		PassDataTableToWaveSubsystem(WaveCompositionDataTable);
	}

	TimberCharacter = Cast<ATimberPlayableCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), ATimberPlayableCharacter::StaticClass()));

	if(TimberCharacter)
	{
		PathTracer_RedrawDelegateBinding();
		TimberCharacter->HandlePlayerDeath_DelegateHandle.AddDynamic(this, &ATimberGameModeBase::FreezeAllAICharacters);
	}
}

void ATimberGameModeBase::PathTracer_RedrawDelegateBinding()
{
	/*
	 * When the build system component on the character spawns a building component at some location
	 * the Game Mode will then Redraw the Path Trace to show the player where they can build.
	 */
	
	if(GetWorld())
	{
		if(TimberCharacter && TimberCharacter->BuildSystemManager)
		{
			TimberCharacter->BuildSystemManager->RedrawPathTraceHandle.AddDynamic(this, 
				&ATimberGameModeBase::HandleRedrawPathTrace);
		}
	}
}

void ATimberGameModeBase::GatherSeedaData()
{
	Seeda = Cast<ATimberSeeda>(UGameplayStatics::GetActorOfClass(GetWorld(), ATimberSeeda::StaticClass()));
	SeedaLocation = Seeda->GetActorLocation();

	//Passing Seeda Class to Save Load Susbsytem for future Seeda Spawning.
	if (USaveLoadSubsystem* SLSubsystem = GetGameInstance()->GetSubsystem<USaveLoadSubsystem>())
	{
		if (Seeda)
		{
			SLSubsystem->SeedaClass = Seeda->GetClass();
		}
	}
}

/*Game State*/
void ATimberGameModeBase::InitializeGameState()
{
	ADieRobotGameStateBase* DieRobotGameState = Cast<ADieRobotGameStateBase>(GetWorld()->GetGameState());
	UDieRobotGameConfigSubsystem* DieRobotGameConfig = GetGameInstance()->GetSubsystem<UDieRobotGameConfigSubsystem>();
	
	if (DieRobotGameState && DieRobotGameConfig)
	{
		//Standard Game with Tutorial
		if (DieRobotGameConfig->GameConfig == EDieRobotGameConfigType::Standard)
		{
			UE_LOG(LogTemp, Warning, TEXT("ATimberGameModeBase - Initialized Standard Game State."))
			DieRobotGameState->OnTutorialStateChange.AddDynamic(this, &ATimberGameModeBase::UpdateTutorialState);
			DieRobotGameState->OnTutorialStateChange.AddDynamic(this, &ATimberGameModeBase::HandleTutorialStateChange);
			
			GetTutorialState();
			
			//Just initiating the Broadcast
			if (TutorialState == ETutorialState::Wake1)
			{
				DieRobotGameState->ChangeTutorialGameState(ETutorialState::Wake1);
				SpawnDummyForTutorial();
			}
		}
		//Game Demo starting at Mid-Game
		else if (DieRobotGameConfig->GameConfig == EDieRobotGameConfigType::MidGameDemo)
		{
			UE_LOG(LogTemp, Warning, TEXT("ATimberGameModeBase - Initialized Mid Game Demo Game State."))
			USaveLoadSubsystem* SaveLoadSubsystem = GetGameInstance()->GetSubsystem<USaveLoadSubsystem>();
			if (SaveLoadSubsystem)
			{
				//Loading the Game with Wave 9 Builds
				SaveLoadSubsystem->LoadGame();

				//Calling to Redraw the Path Trace next tick, after all the building components are built.
				

				//Set to Tutorial Complete / Ensures HUD Knows to Display all Widgets
				DieRobotGameState->ChangeTutorialGameState(ETutorialState::TutorialComplete);
				
				//Setting Wave Number for GDC Mid Game Demo
				GetWaveGameInstanceSubsystem()->SetCurrentWaveNumber(9);

				//TODO:: Currently Unused, But we can use this later to increase enemy health based on wave #.
				//This broadcasts to Enemy Characters the current Wave Number.
				CurrentWaveNumberHandle.Broadcast(GetWaveGameInstanceSubsystem()->CurrentWaveNumber);
				UE_LOG(LogTemp, Warning, TEXT("Broadcasted Current Wave Number: %d. "), GetWaveGameInstanceSubsystem()->CurrentWaveNumber)
				
				//Load all Game Assets
				//Set Inventory to a reasonable rate
				//Set the Wave to the correct Wave
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("GameModeBase - No GameState Set."))
		}
	}
}

void ATimberGameModeBase::GetTutorialState()
{
	ADieRobotGameStateBase* DieRobotGameState = Cast<ADieRobotGameStateBase>(GetWorld()->GetGameState());
	if (DieRobotGameState)
	{
		TutorialState = DieRobotGameState->TutorialState;
	}
}

void ATimberGameModeBase::UpdateTutorialState(ETutorialState NewState)
{
	//Updates Locally on the Game Mode
	TutorialState = NewState;
}

void ATimberGameModeBase::SpawnDummyForTutorial()
{
	FString DummyAssetLocationString = "/Game/Blueprints/Character/TutorialDummy/BP_TutorialDummy.BP_TutorialDummy_C";

	// Load Blueprint asset
	UClass* DummyBPClass = LoadObject<UClass>(nullptr, *DummyAssetLocationString);
    
	const FVector SpawnLocation = FVector(1845.0f, 2916.0f, 150.0f);
	const FRotator SpawnRotation = FRotator(180.f, 0.f, -180.f);
    
	if (DummyBPClass)
	{
		FActorSpawnParameters SpawnParams;
		ATimberEnemyCharacter* DummyActor = GetWorld()->SpawnActor<ATimberEnemyCharacter>(DummyBPClass, SpawnLocation, SpawnRotation, SpawnParams);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load Dummy Blueprint Class."));
	}
}

void ATimberGameModeBase::HandleTutorialStateChange(ETutorialState NewState)
{
	/*if (NewState == ETutorialState::Wake2)
	{
		SpawnLocationMarker();
	}*/
	/*if (NewState == ETutorialState::Wake3)
	{
		UDialogueManager* DialogueManager = GetWorld()->GetGameInstance()->GetSubsystem<UDialogueManager>();
		if (DialogueManager)
		{
			DialogueManager->PlayVoiceover("Molly_Wake_3");
		}
	}*/
	if (NewState == ETutorialState::WaveStart)
	{
		UWaveGameInstanceSubsystem* WaveSubsystem = GetWaveGameInstanceSubsystem();
		if (WaveSubsystem)
		{
			//Starting wave 1 (Tutorial Wave)
			WaveSubsystem->StartWave();
			UE_LOG(LogTemp, Warning, TEXT("GameModeBase - Starting Wave 1."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("GameModeBase - Wave Subsystem Not Found."));
		}
	}
}

void ATimberGameModeBase::PassDataTableToWaveSubsystem(UDataTable* DataTable)
{
	GetWaveGameInstanceSubsystem()->SetWaveCompositionDataTable(DataTable);
	UE_LOG(LogTemp, Warning, TEXT("Game Mode - Received DataTable and Passed to Wave Subsystem"));
}

/* Tells all relying on systems that the character is initialized */
void ATimberGameModeBase::PlayerIsInitialized()
{
	OnCharacterInitialization.Broadcast();
	
	//If we are in mid Game Demo add some currency to player inventory.
	UDieRobotGameConfigSubsystem* DRCS = GetGameInstance()->GetSubsystem<UDieRobotGameConfigSubsystem>();
	if (DRCS && DRCS->GameConfig == EDieRobotGameConfigType::MidGameDemo)
	{
		if (TimberCharacter && TimberCharacter->InventoryManager)
		{
			TimberCharacter->InventoryManager->AddPartsToInventory(10);
			TimberCharacter->InventoryManager->AddMechanismsToInventory(4);
		}
	}
}

void ATimberGameModeBase::SwitchToMainMenu()
{
	//Stopping the Subsystem from continuing to spawn enemies.
	UWaveGameInstanceSubsystem* WaveGameInstanceSubsystem = GetGameInstance()->GetSubsystem<UWaveGameInstanceSubsystem>();
	if (WaveGameInstanceSubsystem)
	{
		WaveGameInstanceSubsystem->FullStop();
	}
	
	//Handles Switching Levels.
	UGameplayStatics::OpenLevel(GetWorld(), FName("StartUp"));
	UE_LOG(LogTemp, Warning, TEXT("TimberGameModeBase - Switching to Main Menu."));
	
}

UWaveGameInstanceSubsystem* ATimberGameModeBase::GetWaveGameInstanceSubsystem()
{
	return GetGameInstance()->GetSubsystem<UWaveGameInstanceSubsystem>();
}

void ATimberGameModeBase::PlayBuildMusic()
{
	UUMusicManagerSubsystem* MusicManager = GetGameInstance()->GetSubsystem<UUMusicManagerSubsystem>();

	if (MusicManager)
	{
		//Delays the playing of the music by InRate - Might be useful for a fade in effect later.
		FTimerHandle DelayHandle;
		GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([MusicManager]()
		{
			MusicManager->PlayMusic("Build1", 1.0f);	
		}), 1.0f, false);
		
	}
}

void ATimberGameModeBase::PlayAttackMusic()
{
	UUMusicManagerSubsystem* MusicManager = GetGameInstance()->GetSubsystem<UUMusicManagerSubsystem>();

	if (MusicManager)
	{
		//Delays the playing of the music by InRate - Might be useful for a fade in effect later.
		FTimerHandle DelayHandle;
		GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([MusicManager]()
		{
			MusicManager->PlayMusic("Attack1", 1.0f);	
		}), 1.0f, false);
		
		//MusicManager->PlayMusic("Build1", 2.0f);
	}
}

void ATimberGameModeBase::GatherAllLabDoors()
{
	AActor* LabActor = UGameplayStatics::GetActorOfClass(GetWorld(), ADynamicLab::StaticClass());
	ADynamicLab* Lab = Cast<ADynamicLab>(LabActor);
	if (Lab)
	{
		UE_LOG(LogTemp, Warning, TEXT("Lab Asset Found. Gathering Lab Doors"));
		TArray<UChildActorComponent*> ChildComponents;
		Lab->GetComponents<UChildActorComponent*>(ChildComponents);
		UE_LOG(LogTemp, Warning, TEXT(" There are %d Child Components"), ChildComponents.Num());
		for (UChildActorComponent* Child : ChildComponents)
		{
			if (Cast<ALabDoorBase>(Child->GetChildActor()))
			{
				ArrayOfLabDoors.Add(Cast<AActor>((Child->GetChildActor())));
				UE_LOG(LogTemp, Warning, TEXT("Gathered Lab Door: %s"), *Child->GetName());
			}
		}
	}
	else
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALabDoorBase::StaticClass(), ArrayOfLabDoors);
		UE_LOG(LogTemp, Warning, TEXT("Could not get Dynamic Lab Actor."));
	}
}

void ATimberGameModeBase::HandleRedrawPathTrace()
{
	RedrawPathTrace();
}


//Used to Freeze all AI Characters when the Player Dies.
void ATimberGameModeBase::FreezeAllAICharacters(bool bIsPlayerDead)
{
	TArray<AActor*> ArrayOfAICharacters;
	UGameplayStatics::GetAllActorsOfClass(this, ATimberEnemyCharacter::StaticClass(), ArrayOfAICharacters);

	for (AActor* Character : ArrayOfAICharacters)
	{
		ATimberEnemyCharacter* CharacterEnemy = Cast<ATimberEnemyCharacter>(Character);

		if (CharacterEnemy)
		{
			//Stopping Enemy Movement
			CharacterEnemy->GetCharacterMovement()->StopMovementImmediately();

			//Stopping Enemy AI Tree Logic
			if (CharacterEnemy->GetController())
			{
				ATimberAiControllerBase* AIController = Cast<ATimberAiControllerBase>(CharacterEnemy->GetController());
				if (AIController)
				{
					UBrainComponent* Brain = AIController->BrainComponent;
					if (Brain)
					{
						Brain->StopLogic("Freezing because Player Death");
					}
				}
			}
		}
	}
}

void ATimberGameModeBase::OpenAllLabDoors()
{
	PlayAttackMusic();
	
	for(AActor* LabDoors : ArrayOfLabDoors)
	{
		ALabDoorBase* LabDoor = Cast<ALabDoorBase>(LabDoors);
		if (LabDoor)
		{
			//GEngine->AddOnScreenDebugMessage(5, 3.0, FColor::Black, "LabDoor Exisits");

			LabDoor->OpenLabDoor(GetWorld()->GetDeltaSeconds());
		}
	}
}

void ATimberGameModeBase::OpenLabDoors()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Mode - Received Broadcast from Wave Subsystem Opening Lab Doors"));
	if(ArrayOfLabDoors.Num() <= 0)
	{
		GatherAllLabDoors();
		OpenAllLabDoors();
	}
	else
	{
		OpenAllLabDoors();
	}
}

void ATimberGameModeBase::CloseAllLabDoors()
{
	PlayBuildMusic();
	for(AActor* LabDoors : ArrayOfLabDoors)
	{
		ALabDoorBase* LabDoor = Cast<ALabDoorBase>(LabDoors);
		if (LabDoor)
		{
			LabDoor->CloseLabDoor(GetWorld()->GetDeltaSeconds());
		}
	}
}

void ATimberGameModeBase::CloseLabDoors()
{
	if(ArrayOfLabDoors.Num() <= 0)
	{
		GatherAllLabDoors();
		CloseAllLabDoors();
	}
	else
	{
		CloseAllLabDoors();
	}
}

void ATimberGameModeBase::HandleWaveComplete(int CompletedWave)
{
	if (CompletedWave == 1)
	{
		ADieRobotGameStateBase* DieRobotGameState = Cast<ADieRobotGameStateBase>(GetWorld()->GetGameState());
		if (DieRobotGameState)
		{
			//Wave 1 Completed, progress to finished Tutorial.
			DieRobotGameState->ChangeTutorialGameState(ETutorialState::WaveComplete);
		}
	}
}


