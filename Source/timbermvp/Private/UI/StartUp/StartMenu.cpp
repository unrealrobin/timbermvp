// Property of Paracosm Industries.


#include "UI/StartUp/StartMenu.h"
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
		LoggedInUserDisplayName = LoginSubsystem->UserInfo.OnlineUserDisplayName;
	}
}
