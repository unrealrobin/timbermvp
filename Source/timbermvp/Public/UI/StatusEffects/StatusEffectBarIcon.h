// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusEffectBarIcon.generated.h"

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UStatusEffectBarIcon : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusEffectsIcons")
	FLinearColor StatusBarIconTint = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f); // Default tint for the icon
};
