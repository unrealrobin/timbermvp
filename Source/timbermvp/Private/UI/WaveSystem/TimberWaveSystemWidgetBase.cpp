// Property of Paracosm Industries. Dont use my shit.


#include "UI/WaveSystem/TimberWaveSystemWidgetBase.h"

#include "GameModes/TimberGameModeBase.h"

void UTimberWaveSystemWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	
}

void UTimberWaveSystemWidgetBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	GameMode = Cast<ATimberGameModeBase>(GetWorld()->GetAuthGameMode());
	CurrentWave = GameMode->CurrentWaveNumber;
	TimeToNextWave = GameMode->TimeToNextWave;

	GameMode->CurrentWaveNumberHandle.AddDynamic(this, &UTimberWaveSystemWidgetBase::UpdateCurrentWave);
	
}

void UTimberWaveSystemWidgetBase::UpdateCurrentWave(float CurrentWaveNumber)
{
	CurrentWave = CurrentWaveNumber;
}

void UTimberWaveSystemWidgetBase::UpdateTimeToNextWave()
{
}
