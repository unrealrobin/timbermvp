// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageNumberWidget.generated.h"

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UDamageNumberWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*UFUNCTION(BlueprintCallable)
	void Init(float DamageAmount, FLinearColor DamageColor);*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Number")
	float DamageNumber = 0.f;
};
