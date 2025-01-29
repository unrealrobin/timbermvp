// Property of Paracosm Industries. Dont use my shit.


#include "GameModes/StartUpGameMode.h"

#include "Kismet/GameplayStatics.h"

void AStartUpGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!StartUpMenu )
	{
		StartUpMenu = CreateWidget<UUserWidget>(GetWorld(), StartUpMenuClass );
		if (StartUpMenu)
		{
			StartUpMenu->AddToViewport();
		}
	}
}

void AStartUpGameMode::SwitchToGameLevel()
{
	if (StartUpMenu)
	{
		StartUpMenu->RemoveFromParent();
		StartUpMenu = nullptr;

		UGameplayStatics::OpenLevel(GetWorld(), FName("TheLab"));
	}
}
