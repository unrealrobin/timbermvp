// Property of Paracosm Industries. Dont use my shit.


#include "UI/Death/TimberDeathWidget.h"

#include "GameModes/TimberGameModeBase.h"
#include "Subsystems/Wave/WaveGameInstanceSubsystem.h"

void UTimberDeathWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	ATimberGameModeBase* GameMode = Cast<ATimberGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		LastCompletedWave = GetGameInstance()->GetSubsystem<UWaveGameInstanceSubsystem>()->CurrentWaveNumber;
	}
}

void UTimberDeathWidget::ResetDeathReason()
{
	//Used on any button press on the Death Widget
	DeathReason = EDeathReason::Default;
}
