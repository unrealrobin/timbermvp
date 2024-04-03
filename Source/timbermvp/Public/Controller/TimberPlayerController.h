// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "TimberPlayerController.generated.h"

class ATimberPlayableCharacter;
class USpringArmComponent;
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


/*Input Actions*/
	UPROPERTY(EditAnywhere)
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere)
	UInputAction* LookUpAction;
	UPROPERTY(EditAnywhere)
	UInputAction* LookRightAction;

	
/*Player Controls*/
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void MoveComplete(const FInputActionValue& Value);
	
	UFUNCTION()
	void LookUp(const FInputActionValue& Value);
	UFUNCTION()
	void LookRight(const FInputActionValue& Value);
	
	

	FInputActionValue InputActionValue;

	UFUNCTION(BlueprintCallable)
	FInputActionValue GetInputActionValue(){return InputActionValue;}

protected:

	virtual void SetupInputComponent() override;
	UPROPERTY(BlueprintReadOnly)
	FVector ForwardMoveDirection;
	UPROPERTY(BlueprintReadOnly)
	FVector RightMoveDirection;

	UPROPERTY(BlueprintReadOnly)
	ATimberPlayableCharacter* TimberCharacter;
	UPROPERTY(BlueprintReadOnly)
	USpringArmComponent* TimberCharacterSpringArmComponent;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> TimberInputMappingContext;
	
	
	
	
};
