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

	/* Delegates */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIsBuildMenuOpen, bool, bIsBuildMenuOpen);

	/*Delegate Handles*/
	UPROPERTY(BlueprintAssignable)
	FIsBuildMenuOpen bIsBuildMenuOpen;
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
	TSubclassOf<UUserWidget> DeathWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* DeathWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
	TSubclassOf<UUserWidget> DeleteBuildingComponentWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* DeleteBuildingComponentWidget;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATimberPlayerController> TimberPlayerController;

	
	UFUNCTION(BlueprintCallable, Category="Build Menu")
	void OpenBuildPanelMenu();
	UFUNCTION(BlueprintCallable, Category="Build Menu")
	void CloseBuildPanelMenu();
	UFUNCTION(BlueprintCallable, Category="Death UI")
	void SwitchToDeathUI();
	UFUNCTION(BlueprintCallable)
	void SwitchToGameUI();
	void ShowDeleteBuildingComponentWidget(float ViewportLocationX, float ViewportLocationY);
protected:
	/*Delegate Listeners*/
	UFUNCTION()
	void HandleBuildPanelMenu(bool IsBuildPanelMenuOpen);
	UFUNCTION()
	void ShouldHideBuildMenu();

	
	
};
