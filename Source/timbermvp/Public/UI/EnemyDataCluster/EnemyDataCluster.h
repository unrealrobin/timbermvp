// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyDataCluster.generated.h"

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UEnemyDataCluster : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Bar")
	UUserWidget* HealthBarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Bar")
	UUserWidget* StatusEffectBarWidget;
	
};
