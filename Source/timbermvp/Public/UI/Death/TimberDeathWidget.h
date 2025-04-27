// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimberDeathWidget.generated.h"

UENUM(BlueprintType) // Allows usage in Blueprints
enum class EDeathReason : uint8
{
	Default UMETA(DisplayName = "Default"), // Enum value with display name
	SeedaDestroyed UMETA(DisplayName = "Seeda Destroyed"),
	KipDestroyed UMETA(DisplayName = "KipDestroyed")
};

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UTimberDeathWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Death Stats" )
	EDeathReason DeathReason = EDeathReason::Default;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateDeathReasonText(EDeathReason DeathReasonText);

	UFUNCTION(BlueprintCallable)
	void ResetDeathReason();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Death Stats")
	int LastCompletedWave = 0;
};
