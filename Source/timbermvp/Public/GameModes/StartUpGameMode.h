// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UI/StartUp/StartMenu.h"
#include "StartUpGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TIMBERMVP_API AStartUpGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> StartUpMenuClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UUserWidget* StartUpMenu;

	UFUNCTION()
	void SetGameState(EGameState InGameState);

	UFUNCTION(BlueprintCallable)
	void SwitchToGameLevel();

	UFUNCTION(BlueprintCallable)
	void SwitchToMidgameDemo();
	
};
