// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TimberHUDBase.generated.h"


/**
 * 
 */
UCLASS()
class TIMBERMVP_API ATimberHUDBase : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	//To Be Set on BP_TimberHUDBase
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
	TSubclassOf<UUserWidget> RootWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* RootWidget;
	
};
