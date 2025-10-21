// Property of Paracosm Industries.


#include "UI/StartUp/StartMenu.h"

#include "Blueprint/WidgetTree.h"
#include "Components/VerticalBox.h"
#include "Subsystems/Online/Login.h"

void UStartMenu::NativeConstruct()
{
	Super::NativeConstruct();

	LoginSubsystem = GetGameInstance()->GetSubsystem<ULogin>();
	if (LoginSubsystem)
	{
		LoginSubsystem->IsPlayerLoggedIn.AddDynamic(this, &UStartMenu::HandleUserLogin);
	}
}

void UStartMenu::HandleUserLogin(bool bIsPlayerLoggedIn)
{
	bIsPlayerLoggedInOnline = bIsPlayerLoggedIn;

	if (bIsPlayerLoggedInOnline)
	{
		LoggedInUserDisplayName = LoginSubsystem->LocalUserInfo.OnlineUserDisplayName;
	}
}

void UStartMenu::HideStartMenuSelections()
{
	if (UWidget* FoundWidget = WidgetTree->FindWidget("StartUpMenuVBox"))
	{
		MenuVBox = Cast<UVerticalBox>(FoundWidget);
		if (MenuVBox)
		{
			TArray<UWidget*> ChildrenWidgets = MenuVBox->GetAllChildren();
			for (UWidget* Child : ChildrenWidgets)
			{
				Child->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}
