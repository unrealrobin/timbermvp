// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Character/TimberCharacterBase.h"
#include "Weapons/TimberWeaponBase.h"
#include "TimberPlayableCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	Unequipped UMETA(DisplayName = "None"),
	AxeEquipped UMETA(DisplayName = "AxeEquipped"),
	ChainsawEquipped UMETA(DisplayName = "ChainsawEquipped"),
	PistolEquipped UMETA(DisplayName = "PistolEquipped"),
	
};

/**
 * 
 */
UCLASS()
class TIMBERMVP_API ATimberPlayableCharacter : public ATimberCharacterBase
{
	GENERATED_BODY()

public:
	//Constructor
	ATimberPlayableCharacter();
	virtual void BeginPlay() override;

	/*Components*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	USpringArmComponent* CameraSpringArm;
	USpringArmComponent* GetSpringArmComponent() {return CameraSpringArm;}

	/*Attributes / Defaults*/
	bool IsRunning = true;
	float MaxRunSpeed = 1000.f;

	/*Weapon Slots*/
	UPROPERTY(EditAnywhere, Category="Weapons")
	ATimberWeaponBase* WeaponOne;
	UPROPERTY(EditAnywhere, Category="Weapons")
	ATimberWeaponBase* WeaponTwo;
	UPROPERTY(EditAnywhere, Category="Weapons")
	ATimberWeaponBase* WeaponThree;

	/*Getters & Setters*/
	//TODO:: Implement Getters and Setters for CurrentWeaponState

protected:
	EWeaponState CurrentWeaponState = EWeaponState::Unequipped;
	
};
