// Property of Paracosm Industries. Dont use my shit.


#include "UI/TimberHUDBase.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Character/TimberSeeda.h"
#include "GameModes/TimberGameModeBase.h"


void ATimberHUDBase::BeginPlay()
{
	Super::BeginPlay();
	InitializeWidgets();
	CharacterAndControllerBindings();
	GameModeBindings();
	//SeedaBinding();

	//Binding to Tutorial States
	InitializeTutorialStateBinding();
	HandleTutorialStateChanges(GetTutorialState());

}

void ATimberHUDBase::InitializeWidgets()
{
	if (RootWidgetClass)
	{
		RootWidget = CreateWidget<UUserWidget>(GetWorld(), RootWidgetClass);
		if (RootWidget)
		{
			RootWidget->AddToViewport(1);
		}
	}
	if (BuildMenuWidgetClass)
	{
		BuildMenuWidget = CreateWidget<UUserWidget>(GetWorld(), BuildMenuWidgetClass);
		if (BuildMenuWidget)
		{
			//Creates the BuildMenuWidget and sets it to hidden. This is good because it can load all its data in the background before even showing it.
			BuildMenuWidget->AddToViewport(10);
			BuildMenuWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	if (AmmoCounterWidgetClass)
	{
		AmmoCounterWidget = CreateWidget<UUserWidget>(GetWorld(), AmmoCounterWidgetClass);
		if (AmmoCounterWidget)
		{
			//Positioning of Widget handles on Canvas Panel in the Widget Blueprint.
			AmmoCounterWidget->AddToViewport(1);
			AmmoCounterWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (KBM_MovementControlsWidgetClass)
	{
		KBM_MovementControlsWidget = CreateWidget<UUserWidget>(GetWorld(), KBM_MovementControlsWidgetClass);
		if (KBM_MovementControlsWidget)
		{
			KBM_MovementControlsWidget->AddToViewport(2);
			KBM_MovementControlsWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (KBM_CombatControlsWidgetClass)
	{
		KBM_CombatControlsWidget = CreateWidget<UUserWidget>(GetWorld(), KBM_CombatControlsWidgetClass);
		if (KBM_CombatControlsWidget)
		{
			KBM_CombatControlsWidget->AddToViewport(2);
			KBM_CombatControlsWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (KBM_BuildControlWidgetClass)
	{
		KBM_BuildControlsWidget = CreateWidget<UUserWidget>(GetWorld(), KBM_BuildControlWidgetClass);
		if (KBM_BuildControlsWidget)
		{
			KBM_BuildControlsWidget->AddToViewport(2);
			KBM_BuildControlsWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}			
}		


void ATimberHUDBase::CharacterAndControllerBindings()
{
	TimberPlayerController = Cast<ATimberPlayerController>(GetWorld()->GetFirstPlayerController());
	if (TimberPlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD has Cached Timber Character Controller"));

		TimberPlayerController->IsBuildPanelOpen.AddDynamic(this, &ATimberHUDBase::HandleBuildPanelMenu);
		TimberPlayerController->ShouldHideBuildMenu.AddDynamic(this, &ATimberHUDBase::CloseBuildPanelMenu);
		TimberPlayerController->HandleDeathUI_DelegateHandle.BindUFunction(this, FName("SwitchToDeathUI"));
		TimberPlayerController->ShowAmmoCounter.AddDynamic(this, &ATimberHUDBase::HandleAmmoCounterVisibility);

		TimberCharacter = Cast<ATimberPlayableCharacter>(
			TimberPlayerController->GetCharacter());

		if (TimberCharacter)
		{
			TimberCharacter->HandleSpawnDeleteIconLocation_DelegateHandle.AddDynamic(
				this, &ATimberHUDBase::ShowDeleteBuildingComponentWidget);
			TimberCharacter->HandleRemoveDeleteIcon_DelegateHandle.AddDynamic(
				this, &ATimberHUDBase::HideDeleteBuildingComponentWidget);
		}
	}
}

void ATimberHUDBase::GameModeBindings()
{
	ATimberGameModeBase* GameMode = Cast<ATimberGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->SwitchToStandardUI.BindUFunction(this, FName("SwitchToGameUI"));
	}
}

// Called by Player using the "B" Key, Listening for the Delegate on the Controller
void ATimberHUDBase::HandleBuildPanelMenu(bool IsBuildPanelMenuOpen)
{
	//Open Menu if it isn't Open.
	//Controller will handle the rest.
	if (BuildMenuWidget->IsVisible())
	{
		return;
	}

	OpenBuildPanelMenu();
}

void ATimberHUDBase::InitializeTutorialStateBinding()
{
	ADieRobotGameStateBase* DieRobotGameState = Cast<ADieRobotGameStateBase>(GetWorld()->GetGameState());
	if (DieRobotGameState)
	{
		DieRobotGameState->OnTutorialStateChange.AddDynamic(this, &ATimberHUDBase::HandleTutorialStateChanges);
	}
}

void ATimberHUDBase::HideWidget(UUserWidget* Widget)
{
	if (Widget)
	{
		Widget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ATimberHUDBase::HideAllChildWidgets(TArray<UUserWidget*> Widgets)
{
	if (Widgets.Num() > 0)
	{
		for (UUserWidget* Widget : Widgets)
		{
			HideWidget(Widget);
		}
	}
}

void ATimberHUDBase::ShowWidget(UUserWidget* Widget)
{
	if (Widget)
	{
		Widget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ATimberHUDBase::OpenBuildPanelMenu()
{
	
	if (BuildMenuWidget)
	{
		BuildMenuWidget->SetVisibility(ESlateVisibility::Visible);
		FInputModeGameAndUI GameAndUIInputMode;

		//We may need to adjust this for focusing.
		TimberPlayerController->SetInputMode(GameAndUIInputMode);
		TimberPlayerController->SetFocusedUserWidget(BuildMenuWidget);
		TimberPlayerController->SetMouseLocation(GetCenterOfScreen().X, GetCenterOfScreen().Y);
		
		//Broadcasts to Player - Controlls Bool that starts Raycasting 
		bIsBuildMenuOpen.Broadcast(true);
		
	}
}

//Called from Building Component Icon in W_BuildingComponentIcon - Stays in Building State
void ATimberHUDBase::CloseBuildPanelMenu()
{
	//If the build menu is open, close it and disable the cursor (Build Mode, Raycasting)
	if (BuildMenuWidget && BuildMenuWidget->IsVisible())
	{
		//Broadcasts to Player - Starts Raycasting
		bIsBuildMenuOpen.Broadcast(false);
		//Hiding the Build Menu - NOT Destroying it.
		BuildMenuWidget->SetVisibility(ESlateVisibility::Hidden);
		
		//Setting the InputMode back to Game Only. Input mode is Changed in the Widget Blueprint in Event Preconstruct.
		FInputModeGameOnly GameOnlyInputMode;
		TimberPlayerController->SetInputMode(GameOnlyInputMode);
		TimberPlayerController->DisableCursor();

		//Only Shows delete icon when the build menu is closed.
		HideDeleteBuildingComponentWidget();
	}
}

FVector2d ATimberHUDBase::GetCenterOfScreen()
{
	int32 ViewportSizeX, ViewportSizeY;
	GetOwningPlayerController()->GetViewportSize(ViewportSizeX, ViewportSizeY);

	return FVector2D(ViewportSizeX * 0.5f, ViewportSizeY * 0.5f);
}

FVector2d ATimberHUDBase::GetViewportSize()
{
	int32 ViewportSizeX, ViewportSizeY;
	GetOwningPlayerController()->GetViewportSize(ViewportSizeX, ViewportSizeY);

	return FVector2d(ViewportSizeX, ViewportSizeY);
}

void ATimberHUDBase::SwitchToDeathUI()
{
	RootWidget->RemoveFromParent();
	if (DeathWidget)
	{
		DeathWidget->SetVisibility(ESlateVisibility::Visible);	
	}
	else
	{
		DeathWidget = CreateWidget<UUserWidget>(GetWorld(), DeathWidgetClass);
		if (DeathWidget)
		{
			DeathWidget->AddToViewport(1);
			DeathWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	
	ATimberGameModeBase* GameMode = Cast<ATimberGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->FreezeAllAICharacters(true);
	}
	
	//TODO:: Make sure we disable the keyboard input when the player dies.
}

void ATimberHUDBase::SwitchToGameUI()
{
	if (DeathWidget)
	{
		DeathWidget->SetVisibility(ESlateVisibility::Hidden);
		UE_LOG(LogTemp, Warning, TEXT("HUD - Hid the Death Widget"));	
	}
	if (RootWidget)
	{
		RootWidget->AddToViewport(1);
	}
}

void ATimberHUDBase::ShowDeleteBuildingComponentWidget(float ViewportLocationX, float ViewportLocationY)
{
	//If widget is up, only update the position of the widget.
	//Parameters are the Screen location of the Impact Point

	//Shifting the Widget 100 Right and 100 Down from the Impact Point. On Screen.
	DeleteWidgetLocation.X = ViewportLocationX + DeleteBuildingComponentWidgetShiftX;
	DeleteWidgetLocation.Y = ViewportLocationY + DeleteBuildingComponentWidgetShiftY;
	
	if (DeleteBuildingComponentWidget)
	{
		
		DeleteBuildingComponentWidget->SetPositionInViewport(DeleteWidgetLocation);
		return;
	}

	//If the widget is not up, create it and add it to the viewport.
	if (DeleteBuildingComponentWidgetClass)
	{
		DeleteBuildingComponentWidget = CreateWidget<UUserWidget>(GetWorld(), DeleteBuildingComponentWidgetClass);
		if (DeleteBuildingComponentWidget)
		{
			DeleteBuildingComponentWidget->AddToViewport(1);

			//Constructing a 2D Vector to set the position of the Widget on the Viewport. For Some reason can not use the Delegate System with FVector2d Type.
			if (DeleteWidgetLocation != FVector2d(0, 0))
			{
				DeleteBuildingComponentWidget->SetPositionInViewport(DeleteWidgetLocation);
			}
		}
	}
}

void ATimberHUDBase::HideDeleteBuildingComponentWidget()
{
	//A Way to remove the DeleteBuildingComponentWidget from the Viewport.
	if (DeleteBuildingComponentWidget)
	{
		DeleteBuildingComponentWidget->RemoveFromParent();
		DeleteBuildingComponentWidget = nullptr;
	}
}

void ATimberHUDBase::HandleAmmoCounterVisibility(bool bShouldShowAmmoCounter)
{
	if (AmmoCounterWidget)
	{
		if (bShouldShowAmmoCounter)
		{
			AmmoCounterWidget->SetVisibility(ESlateVisibility::Visible);
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(1, 3.0, FColor::Green, "Showing Ammo Counter");
			}
		}
		else
		{
			AmmoCounterWidget->SetVisibility(ESlateVisibility::Hidden);
			GEngine->AddOnScreenDebugMessage(1, 3.0, FColor::Green, "Hiding Ammo Counter");
		}
	}
}

/*Tutorial*/

ETutorialState ATimberHUDBase::GetTutorialState()
{
	ADieRobotGameStateBase* DieRobotGameState = Cast<ADieRobotGameStateBase>(GetWorld()->GetGameState());
	if (DieRobotGameState)
	{
		HandleTutorialStateChanges(DieRobotGameState->TutorialState);
		return DieRobotGameState->TutorialState;
	}
	return ETutorialState::Default;
}

void ATimberHUDBase::HandleTutorialStateChanges(ETutorialState NewState)
{
	switch (NewState)
	{
	case ETutorialState::Wake1:
		GetRootWidgetChildrenWidgets();
		HideAllChildWidgets(RootWidgetChildrenWidgets);
		break;
	case ETutorialState::Wake2:
		ShowPlayerHealthWidget();
		ShowWidget(KBM_MovementControlsWidget);
		break;
	case ETutorialState::Wake3:
		ShowSeedaHealthWidget();
		HideWidget(KBM_MovementControlsWidget);
		break;
	case ETutorialState::Combat1:
		ShowWidget(KBM_CombatControlsWidget);
		ShowCrossHairWidget();
		break;
	case ETutorialState::Combat2:
		break;
	case ETutorialState::Parts1:
		UE_LOG(LogTemp, Warning, TEXT("Showing HUD."));
		HideWidget(KBM_CombatControlsWidget);
		ShowInventoryPanelWidget();
		break;
	case ETutorialState::Building1:
		ShowWidget(KBM_BuildControlsWidget);
		break;
	case ETutorialState::Building2:
		break;
	case ETutorialState::Building3:
		break;
	case ETutorialState::WaveStart:
		HideWidget(KBM_BuildControlsWidget);
		//Show Wave Data
		break;
	case ETutorialState::WaveComplete:
		break;
	case ETutorialState::Default:
		break;
	}
}

void ATimberHUDBase::GetRootWidgetChildrenWidgets()
{
	//UE_LOG(LogTemp, Warning, TEXT("Getting Root Widget Children"));
	if (RootWidget)
	{
		//UE_LOG(LogTemp, Warning, TEXT("HUD - Root Widget Exists"));
		if (Cast<UPanelWidget>(RootWidget->GetRootWidget()))
		{
			//Getting the Canvas Panel Widget that stores all the child widgets
			int32 NumOfChildWidgets = Cast<UPanelWidget>(RootWidget->GetRootWidget())->GetChildrenCount();
			for (int32 i = 0; i < NumOfChildWidgets; i++)
			{
				UUserWidget* ChildWidget = Cast<UUserWidget>(Cast<UPanelWidget>(RootWidget->GetRootWidget())->GetChildAt(i));
				if (ChildWidget)
				{
					RootWidgetChildrenWidgets.Add(ChildWidget);
					UE_LOG(LogTemp, Warning, TEXT("Added Child Widget: %s"), *ChildWidget->GetName());
				}
			}
		}
	}
}

UUserWidget* ATimberHUDBase::GetWidgetByClassName(FString ClassName)
{
	if (RootWidgetChildrenWidgets.Num() > 0)
	{
		for (UUserWidget* Widget : RootWidgetChildrenWidgets)
		{
			UE_LOG(LogTemp, Warning, TEXT("Widget Class Name: %s"), *Widget->GetClass()->GetName());
			if (Widget->GetClass()->GetName() == ClassName)
			{
				return Widget;
			}
		}
	}

	return nullptr;
}

void ATimberHUDBase::ShowPlayerHealthWidget()
{
	//TODO::Chance this to the correct name of the Inventory Panel Widget
	FString PlayerHealthWidgetClassName = "W_HealthBar_C";
	//UE_LOG(LogTemp, Warning, TEXT("Player Health Widget Class Name: %s"), *PlayerHealthWidgetClassName);
	UUserWidget* Widget = GetWidgetByClassName(PlayerHealthWidgetClassName);
	ShowWidget(Widget);
	
}

void ATimberHUDBase::ShowSeedaHealthWidget()
{
	FString SeedaHealthWidgetClassName = "W_SeedaHealth_C";
	UUserWidget* Widget = GetWidgetByClassName(SeedaHealthWidgetClassName);
	ShowWidget(Widget);
	
}

void ATimberHUDBase::ShowWaveDataWidget()
{
	FString WaveDataWidgetClassName = "W_WaveInfo_C";
	UUserWidget* Widget = GetWidgetByClassName(WaveDataWidgetClassName);
	ShowWidget(Widget);
}

void ATimberHUDBase::ShowInventoryPanelWidget()
{
	FString InventoryWidgetClassName = "W_TopInventoryBar_C";
	UE_LOG(LogTemp, Warning, TEXT("Player Inventory Widget Class Name: %s"), *InventoryWidgetClassName);
	UUserWidget* Widget = GetWidgetByClassName(InventoryWidgetClassName);
	if(Widget)
	{
		ShowWidget(Widget);
		//UE_LOG(LogTemp, Warning, TEXT("Player Inventory Widget Found"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Inventory Widget Not Found"));	
	}
}

void ATimberHUDBase::ShowCrossHairWidget()
{
	FString CrosshairWidgetClassName = "WBP_Crosshair_C";
	//UE_LOG(LogTemp, Warning, TEXT("Player Health Widget Class Name: %s"), *PlayerHealthWidgetClassName);
	UUserWidget* Widget = GetWidgetByClassName(CrosshairWidgetClassName);
	ShowWidget(Widget);
}
