// Property of Paracosm Industries. Dont use my shit.


#include "Death/UI/TimberDeathWidget.h"

#include "GameModes/TimberGameModeBase.h"

void UTimberDeathWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	ATimberGameModeBase* GameMode = Cast<ATimberGameModeBase>(GetWorld()->GetAuthGameMode());
	if(GameMode)
	{
		LastCompletedWave = GameMode->CurrentWaveNumber;
	}
}