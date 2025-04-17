// Property of Paracosm Industries. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartMenu.generated.h"

class ULogin;

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UStartMenu : public UUserWidget
{
	GENERATED_BODY()


public:

	UPROPERTY()
	ULogin* LoginSubsystem = nullptr;

	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void HandleUserLogin(bool bIsPlayerLoggedIn);

	UPROPERTY(BlueprintReadOnly, Category="Start Menu")
	bool bIsPlayerLoggedInOnline = false;

	UPROPERTY(BlueprintReadOnly, Category="Start Menu")
	FString LoggedInUserDisplayName = "";
};
