// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimberDeathWidget.generated.h"

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UTimberDeathWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativePreConstruct() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Death Stats")
	int LastCompletedWave = 0;
};
