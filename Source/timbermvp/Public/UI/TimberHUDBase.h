// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "Controller/TimberPlayerController.h"
#include "Delegates/Delegate.h"
#include "Delegates/DelegateCombinations.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
	TSubclassOf<UUserWidget> BuildMenuWidgetClass;

	//Set In Blueprints
	UPROPERTY(BlueprintReadOnly)
	UUserWidget* BuildMenuWidget;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATimberPlayerController> TimberPlayerController;

protected:
	/*Delegate Listeners*/
	UFUNCTION()
	void HandleBuildPanelMenu(bool IsBuildPanelMenuOpen);
	UFUNCTION()
	void ShouldHideBuildMenu();

	void OpenBuildPanelMenu();
	void CloseBuildPanelMenu();
	
};
