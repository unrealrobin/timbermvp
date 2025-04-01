// Property of Paracosm Industries. Dont use my shit.


#include "Subsystems/SaveLoad/SaveLoadSubsystem.h"

#include "BuildSystem/Constructs/ConstructBase.h"
#include "BuildSystem/Constructs/TeleportConstruct.h"
#include "Character/TimberPlayableCharacter.h"
#include "Character/TimberSeeda.h"
#include "Components/Inventory/InventoryManagerComponent.h"
#include "GameModes/TimberGameModeBase.h"
#include "Subsystems/Wave/WaveGameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "States/PlayerStateBase.h"
#include "Subsystems/GameConfig/DieRobotGameConfigSubsystem.h"


FString USaveLoadSubsystem::GetSaveSlot()
{
	UDieRobotGameConfigSubsystem* GameConfigSubsystem = GetGameInstance()->GetSubsystem<UDieRobotGameConfigSubsystem>();
	if (GameConfigSubsystem)
	{
		return GameConfigSubsystem->GameConfig == EDieRobotGameConfigType::MidGameDemo ? MidGameDemoSaveSlot : StandardSaveSlot;
	}
	UE_LOG(LogTemp, Warning, TEXT("TimberGameModeBase - Using Standard Save Slot."))
	return StandardSaveSlot;
}

void USaveLoadSubsystem::RegisterBuildable(ABuildableBase* Buildable)
{
	//Adds Key Value Pair
	if (Buildable && Buildable->GetGUID().IsValid())
	{
		GuidToBuildableMap.Add(Buildable->GetGUID(), Buildable);
	}
}

void USaveLoadSubsystem::DeRegisterBuildable(FGuid BuildableGUID)
{
	//Removes Key Value Pair
	if (GuidToBuildableMap.Contains(BuildableGUID))
	{
		GuidToBuildableMap.Remove(BuildableGUID);
	}
}

bool USaveLoadSubsystem::bIsBuildableRegistered(FGuid BuildableGUID)
{

	if (GuidToBuildableMap.Contains(BuildableGUID))
	{
		return true;
	}
	
	return false;
}

void USaveLoadSubsystem::ResolveBuildableReferences(TArray<FBuildableData> BuildableData)
{
	for (FBuildableData& Data : BuildableData)
	{
		//Checking if the Buildable is registered in the GuidToBuildableMap
		if (bIsBuildableRegistered(Data.GUID))
		{
			//Retrieving Instance of the Buildable from Guid from TMap
			ABuildableBase* Buildable = GuidToBuildableMap[Data.GUID];
			if (Buildable)
			{
				//Checking if the Buildable is a Building Component
				if (ATimberBuildingComponentBase* BuildingComponent = Cast<ATimberBuildingComponentBase>(Buildable))
				{
					//Checking if the Building Component has any attached Buildables
					if (Data.AttachedBuildablesArray.Num() > 0)
					{
						//Looping through an array of GUID that Represent some Attached Buildables
						for (FGuid AttachedBuildableGUID : Data.AttachedBuildablesArray)
						{
							//Checking if the Attached Buildable is registered in the GuidToBuildableMap
							if (bIsBuildableRegistered(AttachedBuildableGUID))
							{
								ABuildableBase* AttachedBuildable = GuidToBuildableMap[AttachedBuildableGUID];
								if (AttachedBuildable)
								{
									//Adding the Actual Buildable Instance Back to the AttachedBuildingComponents Array
									BuildingComponent->AttachedBuildingComponents.Add(AttachedBuildable);
								}
							}
						}
					}
					/*
					 * Data is of Type FBuildableData
					 * FBuildableData has GUID's for each Snap Attachment Point.
					 * 
					 * Check if the Data Asset has a Valid GUID for the Snap Attachments
					 */
					if (Data.FrontCenterAttachmentGUID.IsValid())
					{
						//Checking that this GUID is already registered. It should be as all Buildables are spawned and registered before hand.
						if (bIsBuildableRegistered(Data.FrontCenterAttachmentGUID))
						{
							BuildingComponent->FrontCenterAttachment = GuidToBuildableMap[Data.FrontCenterAttachmentGUID];
						}
					}

					if (Data.BackCenterAttachmentGUID.IsValid())
					{
						if (bIsBuildableRegistered(Data.BackCenterAttachmentGUID))
						{
							BuildingComponent->BackCenterAttachment = GuidToBuildableMap[Data.BackCenterAttachmentGUID];
						}
					}
				}
				//Checks for other than Building Components (Traps, Constructs, Ramps, Etc.)
			}
		}
	}
}


/*Save System*/
void USaveLoadSubsystem::SaveCurrentGame()
{
	
	FString SaveSlot = GetSaveSlot();
	
	//Creating an instance of the Save Game Object
	USaveLoadStruct* SaveGameInstance = Cast<USaveLoadStruct>(
		UGameplayStatics::CreateSaveGameObject
		(USaveLoadStruct::StaticClass()));

	SaveBuildableData(SaveGameInstance);
	SaveWaveData(SaveGameInstance);
	SavePlayerData(SaveGameInstance);
	SaveSeedaData(SaveGameInstance);


	//TODO:: Create Dynamic Slot names, User to Input Slot Name or will be populated with Wave Info.
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlot, 0);

	if(GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(1, 3.0, FColor::Red, "USaveLoadSubsystem::SaveCurrentGame() -> Game Saved");
	}
}

void USaveLoadSubsystem::CheckBuildingComponentForSnapAttachments(FBuildableData& BuildableData, ATimberBuildingComponentBase* BuildingComponent)
{
	//Individual Snap Points used for information player if snap is available.
	if (BuildingComponent->FrontBottomAttachment &&
		BuildingComponent->FrontBottomAttachment->GetGUID().IsValid())
	{
		BuildableData.FrontBottomAttachmentGUID = BuildingComponent->FrontBottomAttachment->GetGUID();
		UE_LOG(LogTemp, Warning, TEXT("FrontBottomAttachment Found: %s"), *BuildingComponent->FrontBottomAttachment->GetName());

	}

	if (BuildingComponent->FrontCenterAttachment &&
		BuildingComponent->FrontCenterAttachment->GetGUID().IsValid())
	{
		BuildableData.FrontCenterAttachmentGUID = BuildingComponent->FrontCenterAttachment->GetGUID();
		UE_LOG(LogTemp, Warning, TEXT("FrontCenterAttachment Found: %s"), 
		*BuildingComponent->FrontCenterAttachment->GetName());

	}

	if (BuildingComponent->FrontTopAttachment &&
		BuildingComponent->FrontTopAttachment->GetGUID().IsValid())
	{
		BuildableData.FrontTopAttachmentGUID = BuildingComponent->FrontTopAttachment->GetGUID();
	}

	if (BuildingComponent->FrontRightAttachment &&
		BuildingComponent->FrontRightAttachment->GetGUID().IsValid())
	{
		BuildableData.FrontRightAttachmentGUID = BuildingComponent->FrontRightAttachment->GetGUID();
	}

	if (BuildingComponent->FrontLeftAttachment &&
		BuildingComponent->FrontLeftAttachment->GetGUID().IsValid())
	{
		BuildableData.FrontLeftAttachmentGUID = BuildingComponent->FrontLeftAttachment->GetGUID();
	}

	if (BuildingComponent->BackBottomAttachment &&
		BuildingComponent->BackBottomAttachment->GetGUID().IsValid())
	{
		BuildableData.BackBottomAttachmentGUID = BuildingComponent->BackBottomAttachment->GetGUID();
	}

	if (BuildingComponent->BackCenterAttachment &&
		BuildingComponent->BackCenterAttachment->GetGUID().IsValid())
	{
		BuildableData.BackCenterAttachmentGUID = BuildingComponent->BackCenterAttachment->GetGUID();
		UE_LOG(LogTemp, Warning, TEXT("BackCenterAttachment Found: %s"), *BuildingComponent->BackCenterAttachment->GetName());

	}

	if (BuildingComponent->BackTopAttachment &&
		BuildingComponent->BackTopAttachment->GetGUID().IsValid())
	{
		BuildableData.BackTopAttachmentGUID = BuildingComponent->BackTopAttachment->GetGUID();
	}

	if (BuildingComponent->BackRightAttachment &&
		BuildingComponent->BackRightAttachment->GetGUID().IsValid())
	{
		BuildableData.BackRightAttachmentGUID = BuildingComponent->BackRightAttachment->GetGUID();
	}

	if (BuildingComponent->BackLeftAttachment &&
		BuildingComponent->BackLeftAttachment->GetGUID().IsValid())
	{
		BuildableData.BackLeftAttachmentGUID = BuildingComponent->BackLeftAttachment->GetGUID();
	}
}

void USaveLoadSubsystem::SaveBuildableData(USaveLoadStruct* SaveGameInstance)
{
	if (SaveGameInstance)
	{
		TArray<AActor*> CurrentBuildingComponents;
		UGameplayStatics::GetAllActorsOfClass(
			GetWorld(), ABuildableBase::StaticClass(), CurrentBuildingComponents);

		for (AActor* BuildableActor : CurrentBuildingComponents)
		{
			ABuildableBase* Buildable = Cast<ABuildableBase>(BuildableActor);
			if (Buildable && !BuildableActor->ActorHasTag(FName("ENV")))
			{
				//Creating the Building Component Struct to pass to the Save System's Building Component Array
				FBuildableData BuildableData;
				BuildableData.BuildingComponentClass = BuildableActor->GetClass();
				BuildableData.BuildingComponentTransform = BuildableActor->GetActorTransform();

				//Saving the GUID of this Buildable
				BuildableData.GUID = Buildable->GetGUID();

				//Checking if the Saved Buildable is Building Component (Walls/Floors)
				//Walls and floors have Arrays of attachments that we need to save as an array of GUID's.
				//They also have Snap Attachments that need to be saved.
				if (ATimberBuildingComponentBase* BuildingComponent = Cast<ATimberBuildingComponentBase>(Buildable))
				{
					//Looping through the array of attached Buildable Instances and saving their GUID's to our AttachedBuildablesGUID Array.
					if (BuildingComponent->AttachedBuildingComponents.Num() > 0)
					{
						for (ABuildableBase* AttachedComponent : BuildingComponent->AttachedBuildingComponents)
						{
							BuildableData.AttachedBuildablesArray.Add(AttachedComponent->GetGUID());
						}
					}

					//Checks each potential snap point for the Building Component and saves the GUID of the attached Buildable.
					CheckBuildingComponentForSnapAttachments(BuildableData, BuildingComponent);
				}

				//Linking the Parent Building Component to the Trap
				if (ATrapBase* Trap = Cast<ATrapBase>(Buildable))
				{
					//Saving the GUID of the Parent Building Component
					BuildableData.ParentBuildingComponentGUID = Trap->ParentBuildingComponent->GetGUID();
				}
				//Linking the Parent Building Component to the Construct
				if (AConstructBase* Construct = Cast<AConstructBase>(Buildable))
				{
					//Saving the GUID of the Parent Building Component
					BuildableData.ParentBuildingComponentGUID = Construct->ParentBuildingComponent->GetGUID();
				}
				//Linking the Parent Building Component to the Teleport Construct & the Teleport Pair
				if (ATeleportConstruct* TeleportConstruct = Cast<ATeleportConstruct>(Buildable))
				{
					//Saving the GUID of the Parent Building Component
					BuildableData.ParentBuildingComponentGUID = TeleportConstruct->ParentBuildingComponent->GetGUID();
					BuildableData.TeleportPairGUID = TeleportConstruct->TeleportPair->GetGUID();
				}
				
				
				SaveGameInstance->BuildingComponentsArray.Add(BuildableData);
				
				UE_LOG(LogTemp, Warning, TEXT("Saved Building Component: %s"), *BuildableActor->GetName());
				UE_LOG(LogTemp, Warning, TEXT("Buildable GUID: %s"), *BuildableData.GUID.ToString());
				
			
			}

			
		}
	}
}

void USaveLoadSubsystem::SaveWaveData(USaveLoadStruct* SaveGameInstance)
{
	UWaveGameInstanceSubsystem* WaveSubsystem = GetGameInstance()->GetSubsystem<UWaveGameInstanceSubsystem>();
	
	SaveGameInstance->WaveNumber = WaveSubsystem->CurrentWaveNumber;
	
	UE_LOG(LogTemp, Warning, TEXT("Saved Current Wave Number: %d"), SaveGameInstance->WaveNumber);
}

void USaveLoadSubsystem::SavePlayerData(USaveLoadStruct* SaveGameInstance)
{
	ATimberGameModeBase* GameMode = Cast<ATimberGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		ATimberPlayableCharacter* Character = GameMode->TimberCharacter;
		if (Character)
		{
			/*Save Player Location*/
			SaveGameInstance->PlayerData.PlayerLocation = Character->GetActorLocation();

			/*Save Player Inventory/Currency*/
			APlayerStateBase* PS = Cast<APlayerStateBase>(Character->GetPlayerState());
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
}

void USaveLoadSubsystem::SaveSeedaData(USaveLoadStruct* SaveGameInstance)
{
	/*Saving Seeda Location & Health*/
	ATimberSeeda* Seeda = Cast<ATimberSeeda>(UGameplayStatics::GetActorOfClass(GetWorld(), ATimberSeeda::StaticClass()));
	if (Seeda && SaveGameInstance)
	{
		SaveGameInstance->SeedaData.SeedaLocation = Seeda->GetActorLocation();
		SaveGameInstance->SeedaData.SeedaRotation = Seeda->GetActorRotation();
		SaveGameInstance->SeedaData.SeedaHealth = Seeda->CurrentHealth;
	}
}

/* Load System */
void USaveLoadSubsystem::LoadGame()
{
	UWaveGameInstanceSubsystem* WaveSubsystem = GetGameInstance()->GetSubsystem<UWaveGameInstanceSubsystem>();
	ATimberGameModeBase* GameMode = Cast<ATimberGameModeBase>(GetWorld()->GetAuthGameMode());
	if (WaveSubsystem && GameMode)
	{
		WaveSubsystem->ResetWaveEnemies();

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
			
			GameMode->CloseAllLabDoors();
			WaveSubsystem->CloseBossDoor();
			GameMode->HandleRedrawPathTrace();


			if (GameMode->SwitchToStandardUI.IsBound())
			{
				GameMode->SwitchToStandardUI.Execute();
			}

			//Potential to be called before player controller is bound to this.
			//Needs at least 1 listener to not error.
			if (GameMode->EnableStandardInputMappingContext.IsBound())
			{
				GameMode->EnableStandardInputMappingContext.Execute();
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load Game - No Save/Load Slot."));
		}
	}
}

void USaveLoadSubsystem::LoadBuildingComponents(USaveLoadStruct* LoadGameInstance)
{
	if (LoadGameInstance)
	{
		for (FBuildableData BuildingComponentData : LoadGameInstance->BuildingComponentsArray)
		{
			if (BuildingComponentData.BuildingComponentClass)
			{
				AActor* SpawnedActor = GetWorld()->SpawnActor<ABuildableBase>(
					BuildingComponentData.BuildingComponentClass,
					BuildingComponentData.BuildingComponentTransform.GetLocation(),
					BuildingComponentData.BuildingComponentTransform.GetRotation().Rotator());

				if (ABuildableBase* SpawnedBuildable = Cast<ABuildableBase>(SpawnedActor))
				{
					//Set the Saved GUID back on the newly Spawned Buildable
					SpawnedBuildable->SetGUID(BuildingComponentData.GUID);

					//Add the KV Pair to the TMap
					RegisterBuildable(SpawnedBuildable);
				}

				UE_LOG(LogTemp, Warning, TEXT("Loaded Buildable: %s"), *SpawnedActor->GetName());
			}
		}

		ResolveBuildableReferences(LoadGameInstance->BuildingComponentsArray);
		//TODO:: After Loading and Connecting all building Components redraw the PathTracers.
	}
}

void USaveLoadSubsystem::LoadWaveData(USaveLoadStruct* LoadGameInstance)
{
	UWaveGameInstanceSubsystem* WaveSubsystem = GetGameInstance()->GetSubsystem<UWaveGameInstanceSubsystem>();
	ATimberGameModeBase* GameMode = Cast<ATimberGameModeBase>(GetWorld()->GetAuthGameMode());
	if (LoadGameInstance && WaveSubsystem && GameMode)
	{
		WaveSubsystem->CurrentWaveNumber = LoadGameInstance->WaveNumber;

		UE_LOG(LogTemp, Warning, TEXT("Loaded Current Wave Number: %d"), WaveSubsystem->CurrentWaveNumber);

		GameMode->CurrentWaveNumberHandle.Broadcast(WaveSubsystem->CurrentWaveNumber);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SaveLoadSubsystem - Some Issue with LoadWaveData."));
	}
}

void USaveLoadSubsystem::LoadPlayerState(USaveLoadStruct* LoadGameInstance)
{
	ATimberGameModeBase* GameMode = Cast<ATimberGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		ATimberPlayableCharacter* TimberCharacter = GameMode->TimberCharacter;
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
}

void USaveLoadSubsystem::LoadSeedaData(USaveLoadStruct* LoadGameInstance)
{
	ATimberSeeda* Seeda = Cast<ATimberSeeda>(UGameplayStatics::GetActorOfClass(this, ATimberSeeda::StaticClass()));
	//If Seeda Wasn't destroyed - player only died - destroy the instance of Seeda
	if (Seeda)
	{
		Seeda->Destroy();
	}

	FVector HardCodeSeedaLocation = FVector(-2310.000000,-634.000000,130.000000);
	
	FActorSpawnParameters SpawnParams;
	GetWorld()->SpawnActor<ATimberSeeda>(Seeda->GetClass(),
		HardCodeSeedaLocation,
		LoadGameInstance->SeedaData.SeedaRotation,
		SpawnParams);
}