// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimberWaveSystemWidgetBase.generated.h"


class ATimberGameModeBase;
/**
 * 
 */
UCLASS()
class TIMBERMVP_API UTimberWaveSystemWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	float CurrentWave = 1;
	int TimeToNextWave = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ATimberGameModeBase* GameMode;

protected:

	UFUNCTION()
	void UpdateCurrentWave(float CurrentWaveNumber);

	UFUNCTION()
	void UpdateTimeToNextWave();
	
};
