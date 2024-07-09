// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildSystem/TimberBuildingComponentBase.h"
#include "BuildingComponentPanel.generated.h"

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UBuildingComponentPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Building Components")
	TArray<TSubclassOf<ATimberBuildingComponentBase>> ComponentArray;
	
};
