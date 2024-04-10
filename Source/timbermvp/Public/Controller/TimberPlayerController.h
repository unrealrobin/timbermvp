// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "TimberPlayerController.generated.h"


class IInteractable;
class ATimberPlayableCharacter;
class UCharacterMovementComponent;
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
	UPROPERTY(EditAnywhere)
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere)
	UInputAction* InteractAction;
	UPROPERTY(EditAnywhere)
	UInputAction* LightMeleeAttackAction;
	
	/*Player Controls*/
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void MoveComplete(const FInputActionValue& Value);
	UFUNCTION()
	void LookUp(const FInputActionValue& Value);
	UFUNCTION()
	void LookRight(const FInputActionValue& Value);
	UFUNCTION()
	void CharacterJump(const FInputActionValue& Value);
	UFUNCTION()
	void Interact(const FInputActionValue& Value);
	UFUNCTION()
	void LightMeleeAttack(const FInputActionValue& Value);

	// Stores the value of the Move input action
	// 
	FInputActionValue MoveInputActionValue;

	UFUNCTION(BlueprintCallable)
	FInputActionValue GetMoveInputActionValue(){return MoveInputActionValue;}

	void SetInteractableItem(IInteractable* Item);

protected:

	virtual void SetupInputComponent() override;
	
	UPROPERTY(BlueprintReadOnly)
	FVector ForwardMoveDirection;
	UPROPERTY(BlueprintReadOnly)
	FVector RightMoveDirection;

	UPROPERTY(BlueprintReadOnly)
	ATimberPlayableCharacter* TimberCharacter;
	UPROPERTY(BlueprintReadOnly)
	UCharacterMovementComponent* TimberCharacterMovementComponent;
	UPROPERTY(BlueprintReadOnly)
	ATimberPlayerController* TimberPlayerController;

	/*Camera Controls*/
	UPROPERTY(BlueprintReadOnly)
	USpringArmComponent* TimberCharacterSpringArmComponent;
	float ViewPitchMax = 20.0f;
	float ViewPitchMin = -70.0f;

	/*Jump Controls*/
	UPROPERTY(BlueprintReadWrite)
	bool CanJump = false;
	UFUNCTION(BlueprintCallable)
	void CantJump();

	/*Interactable Items*/
	IInteractable* InteractableItem;
	

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> TimberInputMappingContext;
	
	
	
	
};
