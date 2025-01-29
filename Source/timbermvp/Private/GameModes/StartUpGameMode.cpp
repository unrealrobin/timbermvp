// Property of Paracosm Industries. Dont use my shit.


#include "GameModes/StartUpGameMode.h"

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
