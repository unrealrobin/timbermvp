// Property of Paracosm Industries. Dont use my shit.


#include "GameModes/TimberGameModeBase.h"

#include "BrainComponent.h"
#include "AI/TimberAiControllerBase.h"
#include "Character/TimberSeeda.h"
#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Components/BuildSystem/BuildSystemManagerComponent.h"
#include "Components/Inventory/InventoryManagerComponent.h"
#include "Controller/TimberPlayerController.h"
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
		/*
		 * Ensures that the Dialogue Manager is bound to the Game State once the Game State and Dialogue Manager are both Initialized.
		 */
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
	ATimberSeeda* Seeda = Cast<ATimberSeeda>(UGameplayStatics::GetActorOfClass(GetWorld(), ATimberSeeda::StaticClass()));
	SeedaLocation = Seeda->GetActorLocation();
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
    
	const FVector SpawnLocation = FVector(-1165, 3163.0f, 160.0f);
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
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALabDoorBase::StaticClass(), ArrayOfLabDoors);
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

/*Save System*/
/*FString ATimberGameModeBase::GetSaveSlot()
{
	UDieRobotGameConfigSubsystem* GameConfigSubsystem = GetGameInstance()->GetSubsystem<UDieRobotGameConfigSubsystem>();
	if (GameConfigSubsystem)
	{
		
		 return GameConfigSubsystem->GameConfig == EDieRobotGameConfigType::MidGameDemo ? MidGameDemoSaveSlot : StandardSaveSlot;
	}
	UE_LOG(LogTemp, Warning, TEXT("TimberGameModeBase - Using Standard Save Slot."))
	return StandardSaveSlot;
}

void ATimberGameModeBase::SaveCurrentGame()
{
	
	FString SaveSlot = GetSaveSlot();
	
	//Creating an instance of the Save Game Object
	USaveLoadStruct* SaveGameInstance = Cast<USaveLoadStruct>(
		UGameplayStatics::CreateSaveGameObject
		(USaveLoadStruct::StaticClass()));

	SaveBuildingComponentData(SaveGameInstance);
	SaveWaveData(SaveGameInstance);
	SavePlayerData(SaveGameInstance);
	SaveSeedaData(SaveGameInstance);


	//TODO:: Create Dynamic Slot names, User to Input Slot Name or will be populated with Wave Info.
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlot, 0);

	if(GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(1, 3.0, FColor::Red, "ATimberGameModeBase::SaveCurrentGame() -> Game Saved");
	}
}

void ATimberGameModeBase::SaveBuildingComponentData(USaveLoadStruct* SaveGameInstance)
{
	if (SaveGameInstance)
	{
		TArray<AActor*> CurrentBuildingComponents;
		UGameplayStatics::GetAllActorsOfClass(
			GetWorld(), ABuildableBase::StaticClass(), CurrentBuildingComponents);

		for (AActor* BuildingComponentActors : CurrentBuildingComponents)
		{
			//If the Building Component is an Environment Object - Don't Save it.
			//The environment objects are added by the Developer and should not be saved. Only UGC should be saved.
			if (!BuildingComponentActors->ActorHasTag(FName("ENV")))
			{
				//Creating the Building Component Struct to pass to the Save System's Building Component Array
				FBuildingComponentData BuildingComponentData;
				BuildingComponentData.BuildingComponentClass = BuildingComponentActors->GetClass();
				BuildingComponentData.BuildingComponentTransform = BuildingComponentActors->GetActorTransform();
				SaveGameInstance->BuildingComponentsArray.Add(BuildingComponentData);
				UE_LOG(LogTemp, Warning, TEXT("Saved Building Component: %s"), *BuildingComponentActors->GetName());
			}

			
			
		}
	}
}

void ATimberGameModeBase::SaveWaveData(USaveLoadStruct* SaveGameInstance)
{
	SaveGameInstance->WaveNumber = GetWaveGameInstanceSubsystem()->CurrentWaveNumber;
	
	UE_LOG(LogTemp, Warning, TEXT("Saved Current Wave Number: %d"), SaveGameInstance->WaveNumber);
}

void ATimberGameModeBase::SavePlayerData(USaveLoadStruct* SaveGameInstance)
{
	if (TimberCharacter)
	{
		SaveGameInstance->PlayerData.PlayerLocation = TimberCharacter->GetActorLocation();

		APlayerStateBase* PS = Cast<APlayerStateBase>(TimberCharacter->GetPlayerState());
		if (PS)
		{
			SaveGameInstance->PlayerData.PlayerInventory.NumberOfParts = PS->MainInventory->NumberOfParts;
			SaveGameInstance->PlayerData.PlayerInventory.NumberOfMechanism = PS->MainInventory->NumberOfMechanism;
			SaveGameInstance->PlayerData.PlayerInventory.NumberOfUniques = PS->MainInventory->NumberOfUniques;
			
			UE_LOG(LogTemp, Warning, TEXT("Saved Player Inventory - Parts: %d, Mechanisms: %d, Uniques: %d"),
				SaveGameInstance->PlayerData.PlayerInventory.NumberOfParts,
				SaveGameInstance->PlayerData.PlayerInventory.NumberOfMechanism,
				SaveGameInstance->PlayerData.PlayerInventory.NumberOfUniques);
		}
	}
}

void ATimberGameModeBase::SaveSeedaData(USaveLoadStruct* SaveGameInstance)
{
	ATimberSeeda* Seeda = Cast<ATimberSeeda>(UGameplayStatics::GetActorOfClass(GetWorld(), ATimberSeeda::StaticClass()));
	if (Seeda && SaveGameInstance)
	{
		SaveGameInstance->SeedaData.SeedaLocation = Seeda->GetActorLocation();
		SaveGameInstance->SeedaData.SeedaRotation = Seeda->GetActorRotation();
	}
}

/* Load System #1#
void ATimberGameModeBase::LoadGame()
{
	GetWaveGameInstanceSubsystem()->ResetWaveEnemies();

	//Returns the correct Slot to use based on Game Config
	FString LoadSlot = GetSaveSlot();
	
	USaveLoadStruct* LoadGameInstance = Cast<USaveLoadStruct>(
		UGameplayStatics::LoadGameFromSlot(LoadSlot, 0));

	if (LoadGameInstance)
	{
		LoadBuildingComponents(LoadGameInstance);
		LoadWaveData(LoadGameInstance);
		LoadPlayerState(LoadGameInstance);
		LoadSeedaData(LoadGameInstance);

		CloseAllLabDoors();
		GetWaveGameInstanceSubsystem()->CloseBossDoor();
		HandleRedrawPathTrace();


		if (SwitchToStandardUI.IsBound())
		{
			SwitchToStandardUI.Execute();
		}

		//Potential to be called before player controller is bound to this.
		//Needs at least 1 listener to not error.
		if (EnableStandardInputMappingContext.IsBound())
		{
			EnableStandardInputMappingContext.Execute();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load Game - No Save/Load Slot."));
	}
	
}

void ATimberGameModeBase::LoadBuildingComponents(USaveLoadStruct* LoadGameInstance)
{
	if (LoadGameInstance)
	{
		for (FBuildingComponentData BuildingComponentData : LoadGameInstance->BuildingComponentsArray)
		{
			if (BuildingComponentData.BuildingComponentClass)
			{
				AActor* SpawnedActor = GetWorld()->SpawnActor<ABuildableBase>(
					BuildingComponentData.BuildingComponentClass,
					BuildingComponentData.BuildingComponentTransform.GetLocation(),
					BuildingComponentData.BuildingComponentTransform.GetRotation().Rotator());

				UE_LOG(LogTemp, Warning, TEXT("Loaded Buildable: %s"), *SpawnedActor->GetName());
			}
		}

		//TODO:: This is not working. Why Not.
		GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([this]()
				{
					HandleRedrawPathTrace();  // Call it next tick
				}));
	}
}

void ATimberGameModeBase::LoadWaveData(USaveLoadStruct* LoadGameInstance)
{
	if (LoadGameInstance)
	{
		GetWaveGameInstanceSubsystem()->CurrentWaveNumber = LoadGameInstance->WaveNumber;

		UE_LOG(LogTemp, Warning, TEXT("Loaded Current Wave Number: %d"), GetWaveGameInstanceSubsystem()->CurrentWaveNumber);

		CurrentWaveNumberHandle.Broadcast(GetWaveGameInstanceSubsystem()->CurrentWaveNumber);
	}
}

void ATimberGameModeBase::LoadPlayerState(USaveLoadStruct* LoadGameInstance)
{
	if (TimberCharacter)
	{
		TimberCharacter->SetActorLocation(LoadGameInstance->PlayerData.PlayerLocation);
		TimberCharacter->CurrentHealth = TimberCharacter->MaxHealth;
		TimberCharacter->bIsPlayerDead = false;
		
		//Reverting player Inventory to last save.
		APlayerStateBase* PS = Cast<APlayerStateBase>(TimberCharacter->GetPlayerState());
		if (PS)
		{
			PS->MainInventory->NumberOfParts = LoadGameInstance->PlayerData.PlayerInventory.NumberOfParts;
			PS->MainInventory->NumberOfMechanism = LoadGameInstance->PlayerData.PlayerInventory.NumberOfMechanism;
			PS->MainInventory->NumberOfUniques = LoadGameInstance->PlayerData.PlayerInventory.NumberOfUniques;
			
			//Broadcast update so HUD Reflects Inventory
			TimberCharacter->InventoryManager->UpdateInventoryHandle.Broadcast();

			UE_LOG(LogTemp, Warning, TEXT("Loaded Player Inventory - Parts: %d, Mechanisms: %d, Uniques: %d"),
                PS->MainInventory->NumberOfParts,
                PS->MainInventory->NumberOfMechanism,
                PS->MainInventory->NumberOfUniques);
		}
	}
}

void ATimberGameModeBase::LoadSeedaData(USaveLoadStruct* LoadGameInstance)
{
	ATimberSeeda* Seeda = Cast<ATimberSeeda>(UGameplayStatics::GetActorOfClass(this, ATimberSeeda::StaticClass()));
	//If Seeda Wasn't destroyed - player only died - destroy the instance of Seeda
	if (Seeda)
	{
		Seeda->Destroy();
	}

	FVector HardCodeSeedaLocation = FVector(-2310.000000,-634.000000,130.000000);
	
	FActorSpawnParameters SpawnParams;
	GetWorld()->SpawnActor<ATimberSeeda>(SeedaClass,
		HardCodeSeedaLocation,
		LoadGameInstance->SeedaData.SeedaRotation,
		SpawnParams);
}*/

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


