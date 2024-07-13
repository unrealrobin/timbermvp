// Property of Paracosm Industries. Dont use my shit.


#include "UI/TimberHUDBase.h"
#include "Blueprint/UserWidget.h"

void ATimberHUDBase::BeginPlay()
{
	Super::BeginPlay();
	

	if(RootWidgetClass)
	{
		RootWidget = CreateWidget<UUserWidget>(GetWorld(), RootWidgetClass);
		if(RootWidget)
		{
			RootWidget->AddToViewport(1);
		}
	}

	if(BuildMenuWidgetClass)
	{
		BuildMenuWidget = CreateWidget<UUserWidget>(GetWorld(), BuildMenuWidgetClass);
	}

	//Caching Controller to Listen for Delegate Broadcast
	TimberPlayerController = Cast<ATimberPlayerController>(GetWorld()->GetFirstPlayerController());
	if(TimberPlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD has Cached Timber Character Controller"));

		TimberPlayerController->IsBuildPanelOpen.AddDynamic(this, &ATimberHUDBase::HandleBuildPanelMenu);
		TimberPlayerController->ShouldHideBuildMenu.AddDynamic(this, &ATimberHUDBase::ShouldHideBuildMenu);
	};
	
}

// Called by Player using the "B" Key, Listening for the Delegate on the Controller
void ATimberHUDBase::HandleBuildPanelMenu(bool IsBuildPanelMenuOpen)
{
	IsBuildPanelMenuOpen ? OpenBuildPanelMenu() : CloseBuildPanelMenu();
}

//Called by Player using the "Tab" Key
void ATimberHUDBase::ShouldHideBuildMenu()
{
	//If the build menu is open, close it and disable the cursor (Build Mode, Raycasting)
	if(BuildMenuWidget->IsInViewport())
	{
		TimberPlayerController->DisableCursor();
		CloseBuildPanelMenu();
	}
	else //if the build menu is closed, open it and enable the cursor (Building Menu Mode, No Raycasting)
	{
		TimberPlayerController->EnableCursor();
		OpenBuildPanelMenu();
		
	}
}

void ATimberHUDBase::OpenBuildPanelMenu()
{
	
	if(BuildMenuWidget)
	{
		bIsBuildMenuOpen.Broadcast(true);
		TimberPlayerController->RemoveBuildingComponentProxy();
		BuildMenuWidget->AddToViewport(2);
	}
}

//Called from Building Component Icon in W_BuildingComponentIcon
void ATimberHUDBase::CloseBuildPanelMenu()
{
	if(BuildMenuWidget)
	{
		bIsBuildMenuOpen.Broadcast(false);
		BuildMenuWidget->RemoveFromParent();
		FInputModeGameOnly InputMode;

		//Setting the InputMode back to Game Only. Input mode is Changed in the Widget Blueprint in Event Preconstruct.
		TimberPlayerController->SetInputMode(InputMode);
		TimberPlayerController->DisableCursor();
	}
}
