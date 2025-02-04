// Property of Paracosm Industries. Dont use my shit.


#include "UI/TimberHUDBase.h"
#include "Blueprint/UserWidget.h"
#include "Character/TimberSeeda.h"
#include "Components/BuildSystem/BuildSystemManagerComponent.h"
#include "GameModes/TimberGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void ATimberHUDBase::BeginPlay()
{
	Super::BeginPlay();
	InitializeWidgets();
	CharacterAndControllerBindings();
	GameModeBindings();

}

// Called by Player using the "B" Key, Listening for the Delegate on the Controller
void ATimberHUDBase::HandleBuildPanelMenu(bool IsBuildPanelMenuOpen)
{
	IsBuildPanelMenuOpen ? OpenBuildPanelMenu() : CloseBuildPanelMenu();
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
	}
}

void ATimberHUDBase::CharacterAndControllerBindings()
{
	TimberPlayerController = Cast<ATimberPlayerController>(GetWorld()->GetFirstPlayerController());
	if (TimberPlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD has Cached Timber Character Controller"));

		TimberPlayerController->IsBuildPanelOpen.AddDynamic(this, &ATimberHUDBase::HandleBuildPanelMenu);
		TimberPlayerController->ShouldHideBuildMenu.AddDynamic(this, &ATimberHUDBase::ShouldHideBuildMenu);
		TimberPlayerController->HandleDeathUI_DelegateHandle.BindUFunction(this, FName("SwitchToDeathUI"));


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



void ATimberHUDBase::OpenBuildPanelMenu()
{
	if (BuildMenuWidget)
	{
		bIsBuildMenuOpen.Broadcast(true);
		//TimberPlayerController->RemoveBuildingComponentProxy(); (Function moved to Build System Manager Component - RemoveBuildingComponentProxies_All)
		BuildMenuWidget->AddToViewport(2);
	}
}

//Called from Building Component Icon in W_BuildingComponentIcon
void ATimberHUDBase::CloseBuildPanelMenu()
{
	if (BuildMenuWidget)
	{
		bIsBuildMenuOpen.Broadcast(false);
		BuildMenuWidget->RemoveFromParent();
		FInputModeGameOnly InputMode;

		//Setting the InputMode back to Game Only. Input mode is Changed in the Widget Blueprint in Event Preconstruct.
		TimberPlayerController->SetInputMode(InputMode);
		TimberPlayerController->DisableCursor();

		HideDeleteBuildingComponentWidget();
	}
}

//Called by Player using the "Tab" Key
void ATimberHUDBase::ShouldHideBuildMenu()
{
	//If the build menu is open, close it and disable the cursor (Build Mode, Raycasting)
	if (BuildMenuWidget->IsInViewport())
	{
		if(TimberPlayerController)
		{
			TimberPlayerController->DisableCursor();
		}
		CloseBuildPanelMenu();
	}
	else //if the build menu is closed, open it and enable the cursor (Building Menu Mode, No Raycasting)
	{
		if(TimberPlayerController)
		{
			TimberPlayerController->EnableCursor();
		}
		if(TimberCharacter)
		{
			/*When the player reopens the build menu with a component selected, I want the component to disappear.*/
			TimberCharacter->BuildSystemManager->RemoveBuildingComponentProxies_All();
		}
		OpenBuildPanelMenu();
	}
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
	if (DeleteBuildingComponentWidget)
	{
		FVector2D ViewportLocation = FVector2d(
			ViewportLocationX + DeleteBuildingComponentWidgetShiftX,
			ViewportLocationY + DeleteBuildingComponentWidgetShiftY);
		DeleteBuildingComponentWidget->SetPositionInViewport(ViewportLocation, false);
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
			FVector2D ViewportLocation = FVector2d(0, 0);
			DeleteBuildingComponentWidget->SetPositionInViewport(ViewportLocation, false);
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
