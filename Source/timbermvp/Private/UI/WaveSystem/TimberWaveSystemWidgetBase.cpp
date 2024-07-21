// Property of Paracosm Industries. Dont use my shit.


#include "UI/WaveSystem/TimberWaveSystemWidgetBase.h"

#include "GameModes/TimberGameModeBase.h"

void UTimberWaveSystemWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	GameMode = Cast<ATimberGameModeBase>(GetWorld()->GetAuthGameMode());
	CurrentWave = GameMode->CurrentWaveNumber;
	TimeToNextWaveWidget = GameMode->TimeToNextWave;

	/*Delegate Call from Game Mode*/
	GameMode->CurrentWaveNumberHandle.AddDynamic(this, &UTimberWaveSystemWidgetBase::UpdateCurrentWave);
	
}

void UTimberWaveSystemWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Widget is always checking if a timer is going to the wave timer. if there is less than 15 seconds it updates the time on the widget.
	if(GetWorld()->GetTimerManager().IsTimerActive(GameMode->TimeToNextWaveHandle))
	{
		int TempTime = FMath::CeilToInt(GetWorld()->GetTimerManager().GetTimerRemaining(GameMode->TimeToNextWaveHandle));
		if(TempTime < 15)
		{
			if(TempTime != TimeToNextWaveWidget)
			{
				TimeToNextWaveWidget = TempTime;
			}
		}
	}
	else
	{
		TimeToNextWaveWidget = 0;
	}
}

void UTimberWaveSystemWidgetBase::UpdateCurrentWave(float CurrentWaveNumber)
{
	CurrentWave = CurrentWaveNumber;
}


