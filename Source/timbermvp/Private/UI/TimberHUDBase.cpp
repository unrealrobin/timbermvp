// Property of Paracosm Industries. Dont use my shit.


#include "UI/TimberHUDBase.h"
#include "Blueprint/UserWidget.h"
#include "Character/TimberSeeda.h"
#include "Components/BuildSystem/BuildSystemManagerComponent.h"
#include "GameModes/TimberGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/BuildingComponentPanel.h"

void ATimberHUDBase::BeginPlay()
{
	Super::BeginPlay();
	InitializeWidgets();
	CharacterAndControllerBindings();
	GameModeBindings();
	SeedaBinding();
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

void ATimberHUDBase::SeedaBinding()
{
	ATimberSeeda* Seeda = Cast<ATimberSeeda>(UGameplayStatics::GetActorOfClass(this, ATimberSeeda::StaticClass()));
	if (Seeda)
	{
		Seeda->OnSeedaInteractOverlap.AddDynamic(this, &ATimberHUDBase::ShowSeedaOverlappingToolTip);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Seeda Not Found in the Level."));
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

void ATimberHUDBase::ShowSeedaOverlappingToolTip(bool bShouldShowToolTip)
{
	//Create and show tooltip widget if the player is overlapping with the seeda.
	if (bShouldShowToolTip)
	{
		//Where to show it in the screen space.
		
		FVector2d CenterScreen = GetCenterOfScreen();
		FVector2d SeedaScreenLocation;
		SeedaScreenLocation.X = CenterScreen.X + 400;
		SeedaScreenLocation.Y = CenterScreen.Y;
		
		SeedaOverlapWidget = CreateWidget<UUserWidget>(GetWorld(), SeedaOverlapWidgetClass);
		if (SeedaOverlapWidget)
		{
			SeedaOverlapWidget->AddToViewport(1);
			SeedaOverlapWidget->SetPositionInViewport(SeedaScreenLocation, false);
			SeedaOverlapWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else //Hide the tooltip widget if the player is not overlapping with the seeda.
	{
		if (SeedaOverlapWidget)
		{
			SeedaOverlapWidget->SetVisibility(ESlateVisibility::Hidden);
		}
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
