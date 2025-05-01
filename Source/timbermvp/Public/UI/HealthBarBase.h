// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarBase.generated.h"

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UHealthBarBase : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	FName DisplayName = "SET_BOSS_NAME";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	AActor* ActorReference = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	float CurrentHealth = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	float MaxHealth = 0.f;
	
	
};
