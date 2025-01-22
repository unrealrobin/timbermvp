// Property of Paracosm Industries. Dont use my shit.


#include "UI/WaveSystem/TimberWaveSystemWidgetBase.h"

#include "GameModes/TimberGameModeBase.h"
#include "Subsystems/Wave/WaveGameInstanceSubsystem.h"

void UTimberWaveSystemWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	GetGameInstance()->GetSubsystem<UWaveGameInstanceSubsystem>()->CurrentWaveHandle.AddDynamic(this, &UTimberWaveSystemWidgetBase::UpdateCurrentWave);
	
}

void UTimberWaveSystemWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Widget is always checking if a timer is going to the wave timer. if there is less than 15 seconds it updates the time on the widget.
	// TODO:: Change this to a delegate call from the Wave Subsystem to update the time.
	int TempTime = FMath::CeilToInt(GetWorld()->GetTimerManager().GetTimerRemaining(GetGameInstance()->GetSubsystem<UWaveGameInstanceSubsystem>()->TimeToNextWaveHandle));
	if (TempTime)
	{
		//Reduced unchanging updates to the widget
		if (TempTime != TimeToNextWaveWidget)
		{
			TimeToNextWaveWidget = TempTime;
		}
	}
	else
	{
		if(TimeToNextWaveWidget != 0)
		{
			TimeToNextWaveWidget = 0;
		}
	}
	
}

void UTimberWaveSystemWidgetBase::UpdateCurrentWave(int CurrentWaveNumber_FromSubsystem)
{
	CurrentWave = CurrentWaveNumber_FromSubsystem;
}
