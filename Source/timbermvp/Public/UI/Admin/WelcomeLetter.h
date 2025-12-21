// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Controller/TimberPlayerController.h"
#include "WelcomeLetter.generated.h"

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UWelcomeLetter : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	ATimberPlayerController* DrPlayerController;
};
