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
class AHUD;

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
	UInputAction* EquipWeaponOneAction;
	UPROPERTY(EditAnywhere)
	UInputAction* EquipWeaponTwoAction;
	UPROPERTY(EditAnywhere)
	UInputAction* EquipWeaponThreeAction;
	UPROPERTY(EditAnywhere)
	UInputAction* StandardAction
	;
	
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
	void EquipWeaponOne(const FInputActionValue& Value);
	UFUNCTION()
	void EquipWeaponTwo(const FInputActionValue& Value);
	UFUNCTION()
	void EquipWeaponThree(const FInputActionValue& Value);
	UFUNCTION()
	void StandardAttack(const FInputActionValue& Value);

	// Stores the value of the Move input action
	// 
	FInputActionValue MoveInputActionValue;

	UFUNCTION(BlueprintCallable)
	FInputActionValue GetMoveInputActionValue(){return MoveInputActionValue;}

	void SetInteractableItem(IInteractable* Item);

	/*Controller Rotation*/
	UPROPERTY(BlueprintReadOnly)
	float PitchAngle = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	float YawAngle = 0.0f;


protected:
	virtual void SetupInputComponent() override;
	/*Move Data*/
	UPROPERTY(BlueprintReadOnly)
	FVector CharacterForwardMoveDirection;
	UPROPERTY(BlueprintReadOnly)
	FVector CharacterRightMoveDirection;

	/*Timber References*/
	UPROPERTY(BlueprintReadOnly)
	ATimberPlayableCharacter* TimberCharacter;
	UPROPERTY(BlueprintReadOnly)
	UCharacterMovementComponent* TimberCharacterMovementComponent;
	UPROPERTY(BlueprintReadOnly)
	ATimberPlayerController* TimberPlayerController;

	/*Camera Controls*/
	UPROPERTY(BlueprintReadOnly)
	USpringArmComponent* TimberCharacterSpringArmComponent;
	float ViewPitchMax = 90.0f;
	float ViewPitchMin = -80.0f;
	

	/*Jump Controls*/
	UPROPERTY(BlueprintReadWrite)
	bool CanJump = false;
	UPROPERTY(BlueprintReadWrite)
	bool SwitchToWalking = false;
	UFUNCTION()
	void CanCharacterJump();
	UFUNCTION(BlueprintCallable)
	void JumpComplete();

	/*Interactable Items*/
	IInteractable* InteractableItem;

	/*Attack Data*/
	UPROPERTY(BlueprintReadWrite)
	bool CanAttackAgain = true;

	
	


private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> TimberInputMappingContext;

	UFUNCTION()
	void UnEquipWeapon() const;
	
	
};
