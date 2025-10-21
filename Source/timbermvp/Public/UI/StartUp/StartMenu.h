// Property of Paracosm Industries. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Online/Leaderboards.h"
#include "StartMenu.generated.h"


using namespace UE::Online;
class ULogin;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class TIMBERMVP_API UStartMenu : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void NativeConstruct() override;
	
	UPROPERTY()
	ULogin* LoginSubsystem = nullptr;
	
	UFUNCTION(BlueprintCallable)
	void HandleUserLogin(bool bIsPlayerLoggedIn);
	
	UFUNCTION(BlueprintCallable)
	void HideStartMenuSelections();

protected:
	UPROPERTY(BlueprintReadOnly, Category="Start Menu")
	bool bIsPlayerLoggedInOnline = false;

	UPROPERTY(BlueprintReadOnly, Category="Start Menu")
	FString LoggedInUserDisplayName = "";

private:
	UPROPERTY()
	UVerticalBox* MenuVBox = nullptr;
};
