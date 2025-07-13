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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damage Number")
	bool bIsDamage = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Text")
	FName EffectName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Number")
	float DamageNumber = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Number")
	float DamageSize = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Number")
	FLinearColor DamageColor = FLinearColor::White;
};
