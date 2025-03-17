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
	void InitializeWidgets();
	void CharacterAndControllerBindings();
	void GameModeBindings();
	//void SeedaBinding();
	virtual void BeginPlay() override;

	/* Delegates */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIsBuildMenuOpen, bool, bIsBuildMenuOpen);

	/*Delegate Handles*/
	UPROPERTY()
	FIsBuildMenuOpen bIsBuildMenuOpen;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
	TSubclassOf<UUserWidget> SeedaOverlapWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* SeedaOverlapWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
	TSubclassOf<UUserWidget> AmmoCounterWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* AmmoCounterWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
	TSubclassOf<UUserWidget> KBM_MovementControlsWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* KBM_MovementControlsWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
	TSubclassOf<UUserWidget> KBM_CombatControlsWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* KBM_CombatControlsWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
	TSubclassOf<UUserWidget> KBM_BuildControlWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* KBM_BuildControlsWidget;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATimberPlayerController> TimberPlayerController;
	
	UFUNCTION(BlueprintCallable, Category = "Build Menu")
	void OpenBuildPanelMenu();
	
	UFUNCTION(BlueprintCallable, Category = "Build Menu")
	void CloseBuildPanelMenu();
	
	UFUNCTION(BlueprintCallable, Category="Death UI")
	void SwitchToDeathUI();
	
	UFUNCTION(BlueprintCallable)
	void SwitchToGameUI();
	
	UFUNCTION(Category="Action UI")
	void ShowDeleteBuildingComponentWidget(float ViewportLocationX, float ViewportLocationY);
	
	UFUNCTION()
	void HideDeleteBuildingComponentWidget();

	UFUNCTION()
	void HandleAmmoCounterVisibility(bool bShouldShowAmmoCounter);
	
	FVector2d GetCenterOfScreen();
	FVector2d GetViewportSize();

	UFUNCTION()
	void ShowAllGameWidgets();

protected:
	UPROPERTY()
	ATimberPlayableCharacter* TimberCharacter;
	
	/*Delegate Listeners*/
	UFUNCTION()
	void HandleBuildPanelMenu(bool IsBuildPanelMenuOpen);

	//Vars to shift in the X & Y direction when the DeleteBuildingComponentWidget is spawned. Shifted from Center.
	FVector2d DeleteWidgetLocation = FVector2d(0, 0);
	float DeleteBuildingComponentWidgetShiftX = 100.f;
	float DeleteBuildingComponentWidgetShiftY = 100.f;

private:
	/* Tutorial State Handling*/
	UFUNCTION()
	void InitializeTutorialStateBinding();

	UFUNCTION()
	ETutorialState GetTutorialState();

	UFUNCTION()
	void HandleTutorialStateChanges(ETutorialState NewState);

	UPROPERTY()
	TArray<UUserWidget*> RootWidgetChildrenWidgets;

	void GetRootWidgetChildrenWidgets();

	UUserWidget* GetWidgetByClassName(FString ClassName);

	void HideWidget(UUserWidget* Widget);
	void ShowWidget(UUserWidget* Widget);
	void HideAllChildWidgets(TArray<UUserWidget*> Widgets);
	void ShowCrossHairWidget();
	void ShowInventoryPanelWidget();
	void ShowPlayerHealthWidget();
	void ShowSeedaHealthWidget();
	void ShowWaveDataWidget();
	
};







