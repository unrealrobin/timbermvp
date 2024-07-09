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

void ATimberHUDBase::HandleBuildPanelMenu(bool IsBuildPanelMenuOpen)
{
	IsBuildPanelMenuOpen ? OpenBuildPanelMenu() : CloseBuildPanelMenu();
}

void ATimberHUDBase::ShouldHideBuildMenu()
{
	if(BuildMenuWidget->IsInViewport())
	{
		CloseBuildPanelMenu();
		
		TimberPlayerController->DisableCursor();
	}
	else
	{
		OpenBuildPanelMenu();
		TimberPlayerController->EnableCursor();
	}
}

void ATimberHUDBase::OpenBuildPanelMenu()
{
	
	if(BuildMenuWidget)
	{
		BuildMenuWidget->AddToViewport(2);
		BuildMenuWidget->SetFocus();
	}
}

void ATimberHUDBase::CloseBuildPanelMenu()
{
	if(BuildMenuWidget)
	{
		BuildMenuWidget->RemoveFromParent();
	}
}
