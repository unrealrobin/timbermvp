// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Character/TimberCharacterBase.h"
#include "Weapons/TimberWeaponBase.h"
#include "TimberPlayableCharacter.generated.h"

class ATimberBuildSystemManager;
class USpringArmComponent;
class UCameraComponent;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	Unequipped UMETA(DisplayName = "None"),
	AxeEquipped UMETA(DisplayName = "AxeEquipped"),
	ChainsawEquipped UMETA(DisplayName = "ChainsawEquipped"),
	RangedEquipped UMETA(DisplayName = "RangedEquipped"),
	
};

UENUM(BlueprintType)
enum class ECharacterState: uint8
{
	Standard UMETA(DisplayName = "Standard"),
	Building UMETA(DisplayName = "Building")
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

	//CharacterState
	ECharacterState CharacterState = ECharacterState::Standard;

	/*Components*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	USpringArmComponent* CameraSpringArm;
	USpringArmComponent* GetSpringArmComponent() {return CameraSpringArm;}

	/*Attributes / Defaults*/
	bool IsRunning = true;
	float MaxRunSpeed = 1000.f;

	/*Animation Properties*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation")
	bool IsNowJumping = false;

	/*Weapon Slots*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapons")
	TSubclassOf<ATimberWeaponBase> WeaponOne;
	UPROPERTY(BlueprintReadOnly, Category="Weapons")
	ATimberWeaponBase* WeaponOneInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapons")
	TSubclassOf<ATimberWeaponBase> WeaponTwo;
	UPROPERTY(BlueprintReadOnly, Category="Weapons")
	ATimberWeaponBase* WeaponTwoInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapons")
	TSubclassOf<ATimberWeaponBase> WeaponThree;
	UPROPERTY(BlueprintReadOnly, Category="Weapons")
	ATimberWeaponBase* WeaponThreeInstance;

	/*Getters & Setters*/
	EWeaponState GetCurrentWeaponState() const {return CurrentWeaponState;} 
	void SetCurrentWeaponState(EWeaponState NewWeaponState);
	void SetCurrentlyEquippedWeapon(ATimberWeaponBase* Weapon);
	ATimberWeaponBase* GetCurrentlyEquippedWeapon() const {return CurrentlyEquippedWeapon;}

	/*Build System*/
	//TODO:: Fix the Issue here with Instantiating the BuildSystemManager
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Building")
	TSubclassOf<ATimberBuildSystemManager> BuildSystemManagerClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Building")
	ATimberBuildSystemManager* BuildSystemManagerInstance;*/
	void PerformRaycast();
	UPROPERTY(EditAnywhere, Category="Building")
	float BuildRaycastDistance = 1000.f;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Weapon State")
	EWeaponState CurrentWeaponState = EWeaponState::Unequipped;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATimberWeaponBase> CurrentlyEquippedWeapon;

	virtual void Tick(float DeltaSeconds) override;


	
	
	
	
};
