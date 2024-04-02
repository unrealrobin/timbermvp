// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "TimberPlayerController.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class TIMBERMVP_API ATimberPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UInputAction* MoveAction;

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	FInputActionValue InputActionValue;

	UFUNCTION(BlueprintCallable)
	FInputActionValue GetInputActionValue(){return InputActionValue;}

protected:

	virtual void SetupInputComponent() override;
	UPROPERTY(BlueprintReadOnly)
	FVector ForwardMoveDirection;
	UPROPERTY(BlueprintReadOnly)
	FVector RightMoveDirection;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> TimberInputMappingContext;
	
	
	
	
};
