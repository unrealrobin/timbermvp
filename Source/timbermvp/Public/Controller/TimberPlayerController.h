// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Character/TimberPlayableCharacter.h"
#include "EnhancedInputComponent.h"
#include "TimberPlayerController.generated.h"



class ATimberBuildSystemManager;
class IInteractable;
class UCharacterMovementComponent;
class USpringArmComponent;
class UInputMappingContext;
class AHUD;
class UEnhancedInputLocalPlayerSubsystem;




/**
 * 
 */
UCLASS()
class TIMBERMVP_API ATimberPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	/*Delegates*/
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponStateChange, EWeaponState, NewState); 
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildMenuToggle, bool, bIsBuildPanelOpen);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHideBuildMenu);
	DECLARE_DYNAMIC_DELEGATE(FHandleDeathUI);
	
	
	/*DelegateHandles*/
	UPROPERTY(BlueprintAssignable)
	FOnWeaponStateChange WeaponState;
	UPROPERTY(BlueprintAssignable)
	FOnBuildMenuToggle IsBuildPanelOpen;
	UPROPERTY(BlueprintAssignable)
	FOnHideBuildMenu ShouldHideBuildMenu;
	FHandleDeathUI HandleDeathUI_DelegateHandle;
	

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
	UInputAction* StandardAction;
	UPROPERTY(EditAnywhere)
	UInputAction* ToggleBuildModeAction;
	UPROPERTY(EditAnywhere)
	UInputAction* RotateBuildingComponentAction;
	UPROPERTY(EditAnywhere)
	UInputAction* PlaceBuildingComponentAction;
	UPROPERTY(EditAnywhere)
	UInputAction* HideBuildMenuAction;
	UPROPERTY(EditAnywhere)
	UInputAction* DeleteBuildingComponentAction;
	
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
	UFUNCTION()
	void ToggleBuildMode(const FInputActionValue& Value);
	UFUNCTION()
	void RemoveBuildingComponentProxy();
	UFUNCTION()
	void RotateBuildingComponent(const FInputActionValue& Value);
	UFUNCTION()
	void PlaceBuildingComponent(const FInputActionValue& Value);
	UFUNCTION()
	void HideBuildMenu(const FInputActionValue& Value);
	UFUNCTION()
	void DeleteBuildingComponent(const FInputActionValue& Value);

	// Stores the value of the Move input action
	FInputActionValue MoveInputActionValue;

	UFUNCTION(BlueprintCallable)
	FInputActionValue GetMoveInputActionValue(){return MoveInputActionValue;}

	void SetInteractableItem(IInteractable* Item);

	/*Controller Rotation*/
	UPROPERTY(BlueprintReadOnly)
	float PitchAngle = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	float YawAngle = 0.0f;
	
	/*Cursor*/
	void EnableCursor();
	void DisableCursor();

	void MovePlayerToStartLocation();

	//Input Mapping Contexts
	//This needs to be public because it gets called on the BP version of this class.
	UFUNCTION(BlueprintCallable) 
	void EnableStandardKeyboardInput();

protected:
	
	virtual void SetupInputComponent() override;
	
	/*Move Data*/
	UPROPERTY(BlueprintReadOnly)
	FVector CharacterForwardMoveDirection;
	UPROPERTY(BlueprintReadOnly)
	FVector CharacterRightMoveDirection;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector PlayerStartLocation;

	/*Timber References*/
	UPROPERTY(BlueprintReadOnly)
	ATimberPlayableCharacter* TimberCharacter;
	UPROPERTY(BlueprintReadOnly)
	UCharacterMovementComponent* TimberCharacterMovementComponent;

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

	/*Build Controls*/
	//Gets set in the BeginPlay of the TimberPlayerController.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ATimberBuildSystemManager* TimberBuildSystemManager;
	void ExitBuildMode(ECharacterState NewState);
	UFUNCTION()
	void OpenBuildModeSelectionMenu();
	UFUNCTION()
	void CloseBuildModeSelectionMenu();

	/*Death*/
	UFUNCTION()
	void HandlePlayerDeath(bool bIsPlayerDead);

private:
	/*Enhanced Input Subsystem*/
	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* Subsystem;

	/*Input Mapping Contexts for Different Character States*/
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> StandardInputMappingContext;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> BuildModeInputMappingContext;
	
	void DisableAllKeyboardInput();
	

	UFUNCTION()
	void UnEquipWeapon() const;
	
	
};
