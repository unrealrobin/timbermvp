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
	/*
	* We need to check if this is bound because, when the widget is removed and readded to the screen,
	It tries to rebind the delegate again which causes an error. so We first check to ensure it isn't bound
	before we wind the delegate to the handle.
	 *
	 * One of the ways this error happens is when the character dies and the UI changes and so the UI is recreated on load.
	 *
	 *
	 *
	*/
	
	if(!GameMode->CurrentWaveNumberHandle.IsBound())
	{
		GameMode->CurrentWaveNumberHandle.AddDynamic(this, &UTimberWaveSystemWidgetBase::UpdateCurrentWave);
	}
	
	
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


