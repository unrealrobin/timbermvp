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
#include "SaveSystem/TimberSaveSystem.h"
#include "Subsystems/Dialogue/DialogueManager.h"
#include "Subsystems/Music/UMusicManagerSubsystem.h"
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
		GetWaveGameInstanceSubsystem()->SaveCurrentGameHandle.AddDynamic(this, &ATimberGameModeBase::SaveCurrentGame);
		GetWaveGameInstanceSubsystem()->HandleWaveComplete.AddDynamic(this, &ATimberGameModeBase::HandleWaveComplete);
		/*Subscribing to Player Death Delegate Signature*/
	}

	// Initial Wave Broadcast - FOR UI Wave System Widget I think
	CurrentWaveNumberHandle.Broadcast(GetWaveGameInstanceSubsystem()->CurrentWaveNumber);
	
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
	if (DieRobotGameState)
	{
		UE_LOG(LogTemp, Warning, TEXT("ATimberGameModeBase - Initialized Game State."))
		DieRobotGameState->OnTutorialStateChange.AddDynamic(this, &ATimberGameModeBase::UpdateGameState);
		DieRobotGameState->OnTutorialStateChange.AddDynamic(this, &ATimberGameModeBase::HandleGameStateChange);
		GetGameState();
	}

	if (TutorialState == ETutorialState::Wake1)
	{
		UDialogueManager* DialogueManager = GetWorld()->GetGameInstance()->GetSubsystem<UDialogueManager>();
		if (DialogueManager)
		{
			DialogueManager->PlayVoiceover("Wake1");
		}
	}

	
}

void ATimberGameModeBase::GetGameState()
{
	ADieRobotGameStateBase* DieRobotGameState = Cast<ADieRobotGameStateBase>(GetWorld()->GetGameState());
	if (DieRobotGameState)
	{
		TutorialState = DieRobotGameState->TutorialState;
	}
}

void ATimberGameModeBase::UpdateGameState(ETutorialState NewState)
{
	TutorialState = NewState;
}

void ATimberGameModeBase::HandleGameStateChange(ETutorialState NewState)
{
	if (NewState == ETutorialState::Wake2)
	{
		SpawnLocationMarker();
	}
	if (NewState == ETutorialState::Wake3)
	{
		UDialogueManager* DialogueManager = GetWorld()->GetGameInstance()->GetSubsystem<UDialogueManager>();
		if (DialogueManager)
		{
			DialogueManager->PlayVoiceover("Molly_Wake_3");
		}
	}
	if (NewState == ETutorialState::WaveStart)
	{
		UWaveGameInstanceSubsystem* WaveSubsystem = GetWaveGameInstanceSubsystem();
		if (WaveSubsystem)
		{
			//Starting wave 1 (Tutorial Wave)
			WaveSubsystem->StartWave();
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

void ATimberGameModeBase::SpawnLocationMarker()
{
	ATimberSeeda* Seeda = Cast<ATimberSeeda>(UGameplayStatics::GetActorOfClass(GetWorld(), ATimberSeeda::StaticClass()));
	if (Seeda)
	{
		SeedaLocation = Seeda->GetActorLocation();
		SeedaLocation.Z -= 100.0f;

		FActorSpawnParameters SpawnParams;

		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(LocationMarker, SeedaLocation, FRotator(0, 0, 0), SpawnParams);
	}
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
void ATimberGameModeBase::SaveCurrentGame()
{
	//Creating an instance of the Save Game Object
	UTimberSaveSystem* SaveGameInstance = Cast<UTimberSaveSystem>(
		UGameplayStatics::CreateSaveGameObject
		(UTimberSaveSystem::StaticClass()));

	SaveBuildingComponentData(SaveGameInstance);
	SaveWaveData(SaveGameInstance);
	SavePlayerData(SaveGameInstance);
	SaveSeedaData(SaveGameInstance);


	//TODO:: Create Dynamic Slot names, User to Input Slot Name or will be populated with Wave Info.
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("Demo Timber Save 1"), 0);

	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 3.0, FColor::Red, "ATimberGameModeBase::SaveCurrentGame() -> Game Saved");
	}
}

void ATimberGameModeBase::SaveBuildingComponentData(UTimberSaveSystem* SaveGameInstance)
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

void ATimberGameModeBase::SaveWaveData(UTimberSaveSystem* SaveGameInstance)
{
	SaveGameInstance->WaveNumber = GetWaveGameInstanceSubsystem()->CurrentWaveNumber;
	
	UE_LOG(LogTemp, Warning, TEXT("Saved Current Wave Number: %d"), SaveGameInstance->WaveNumber);
}

void ATimberGameModeBase::SavePlayerData(UTimberSaveSystem* SaveGameInstance)
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

void ATimberGameModeBase::SaveSeedaData(UTimberSaveSystem* SaveGameInstance)
{
	ATimberSeeda* Seeda = Cast<ATimberSeeda>(UGameplayStatics::GetActorOfClass(GetWorld(), ATimberSeeda::StaticClass()));
	if (Seeda && SaveGameInstance)
	{
		SaveGameInstance->SeedaData.SeedaLocation = Seeda->GetActorLocation();
		SaveGameInstance->SeedaData.SeedaRotation = Seeda->GetActorRotation();
	}
}

/* Load System */
void ATimberGameModeBase::LoadGame()
{
	GetWaveGameInstanceSubsystem()->ResetWaveEnemies();
	//Needs the Slot Name and the User Index
	UTimberSaveSystem* LoadGameInstance = Cast<UTimberSaveSystem>(
		UGameplayStatics::LoadGameFromSlot(TEXT("Demo Timber Save 1"), 0));
	
	LoadBuildingComponents(LoadGameInstance);
	LoadWaveData(LoadGameInstance);
	LoadPlayerState(LoadGameInstance);
	LoadSeedaData(LoadGameInstance);

	CloseAllLabDoors();
	GetWaveGameInstanceSubsystem()->CloseBossDoor();
	HandleRedrawPathTrace();
	
	SwitchToStandardUI.Execute();
	EnableStandardInputMappingContext.Execute();
}

void ATimberGameModeBase::LoadBuildingComponents(UTimberSaveSystem* LoadGameInstance)
{
	if (LoadGameInstance)
	{
		for (FBuildingComponentData BuildingComponentData : LoadGameInstance->BuildingComponentsArray)
		{
			if (BuildingComponentData.BuildingComponentClass)
			{
				GetWorld()->SpawnActor<ABuildableBase>(
					BuildingComponentData.BuildingComponentClass,
					BuildingComponentData.BuildingComponentTransform.GetLocation(),
					BuildingComponentData.BuildingComponentTransform.GetRotation().Rotator());
			}
		}
	}
}

void ATimberGameModeBase::LoadWaveData(UTimberSaveSystem* LoadGameInstance)
{
	if (LoadGameInstance)
	{
		GetWaveGameInstanceSubsystem()->CurrentWaveNumber = LoadGameInstance->WaveNumber;

		UE_LOG(LogTemp, Warning, TEXT("Loaded Current Wave Number: %d"), GetWaveGameInstanceSubsystem()->CurrentWaveNumber);

		CurrentWaveNumberHandle.Broadcast(GetWaveGameInstanceSubsystem()->CurrentWaveNumber);
	}
}

void ATimberGameModeBase::LoadPlayerState(UTimberSaveSystem* LoadGameInstance)
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

void ATimberGameModeBase::LoadSeedaData(UTimberSaveSystem* LoadGameInstance)
{
	ATimberSeeda* Seeda = Cast<ATimberSeeda>(UGameplayStatics::GetActorOfClass(this, ATimberSeeda::StaticClass()));
	//If Seeda Wasn't destroyed - player only died - destroy the instance of Seeda
	if (Seeda)
	{
		Seeda->Destroy();
	}
	
	FActorSpawnParameters SpawnParams;
	GetWorld()->SpawnActor<ATimberSeeda>(SeedaClass, LoadGameInstance->SeedaData.SeedaLocation, 
	LoadGameInstance->SeedaData
	.SeedaRotation, SpawnParams);
}

void ATimberGameModeBase::OpenAllLabDoors()
{
	PlayAttackMusic();
	
	for(AActor* LabDoors : ArrayOfLabDoors)
	{
		ALabDoorBase* LabDoor = Cast<ALabDoorBase>(LabDoors);
		if (LabDoor)
		{
			GEngine->AddOnScreenDebugMessage(5, 3.0, FColor::Black, "LabDoor Exisits");

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


