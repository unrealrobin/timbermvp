// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/BuildableBase.h"
#include "Character/TimberCharacterBase.h"
#include "Weapons/TimberWeaponBase.h"
#include "TimberPlayableCharacter.generated.h"

class ATimberPlayerController;
class ATimberWeaponRangedBase;
class ATimberWeaponMeleeBase;
class UBuildSystemManagerComponent;
class ATimberBuildingComponentBase;
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player Controller")
	ATimberPlayerController* RaycastController;

	//Delegates
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHandlePlayerDeath, bool, bIsPlayerDead);

	FHandlePlayerDeath HandlePlayerDeath_DelegateHandle;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
		FHandleSpawnDeleteIconLocation, float, ViewportLocationX, float, ViewportLocationY);

	FHandleSpawnDeleteIconLocation HandleSpawnDeleteIconLocation_DelegateHandle;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHandleRemoveDeleteIcon);

	FHandleRemoveDeleteIcon HandleRemoveDeleteIcon_DelegateHandle;

	//Constructor
	ATimberPlayableCharacter();
	virtual void BeginPlay() override;

	//CharacterState
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character State")
	ECharacterState CharacterState = ECharacterState::Standard;

	/*Components*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	USpringArmComponent* CameraSpringArm;
	USpringArmComponent* GetSpringArmComponent() { return CameraSpringArm; }
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Build System")
	UBuildSystemManagerComponent* BuildSystemManager;

	/*Attributes / Defaults*/
	bool IsRunning = true;
	float MaxRunSpeed = 2000.f;

	/*Animation Montages*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Animation")
	UAnimMontage* DeathMontage;

	/*Animation Properties*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation")
	bool IsNowJumping = false;

	/*Weapon Slots*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapons")
	TSubclassOf<ATimberWeaponBase> WeaponOne;
	UPROPERTY(BlueprintReadOnly, Category="Weapons")
	ATimberWeaponMeleeBase* WeaponOneInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapons")
	TSubclassOf<ATimberWeaponBase> WeaponTwo;
	UPROPERTY(BlueprintReadOnly, Category="Weapons")
	ATimberWeaponMeleeBase* WeaponTwoInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapons")
	TSubclassOf<ATimberWeaponBase> WeaponThree;
	UPROPERTY(BlueprintReadOnly, Category="Weapons")
	ATimberWeaponRangedBase* WeaponThreeInstance;

	/*Getters & Setters*/
	EWeaponState GetCurrentWeaponState() const { return CurrentWeaponState; }
	void SetCurrentWeaponState(EWeaponState NewWeaponState);
	void SetCurrentlyEquippedWeapon(ATimberWeaponBase* Weapon);
	bool HandleShowDeleteWidget(FHitResult HitResult);
	void HandleRaycastHitConditions(bool bHits);
	ATimberWeaponBase* GetCurrentlyEquippedWeapon() const { return CurrentlyEquippedWeapon; }

	/*Build System*/
	void PerformBuildSystemRaycast();
	void HandleTrapPlacement();
	bool HandleBuildingComponentPlacement();

	UPROPERTY(EditAnywhere, Category="Build System Info")
	float BuildRaycastDistance = 1000.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Build System Info")
	bool ShouldRaycast = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Build System Info")
	ATimberBuildingComponentBase* HoveredBuildingComponent;

	UPROPERTY(VisibleAnywhere, Category="Build System Info")
	TArray<FHitResult> HitResults;
	FVector BuildingComponentImpactPoint;

	UFUNCTION()
	void ExitBuildMode();

	/*Damage*/
	void PlayerTakeDamage(float DamageAmount);

	/*Death*/
	UFUNCTION()
	void HandlePlayerDeath();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health")
	bool bIsPlayerDead = false;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Weapon State")
	EWeaponState CurrentWeaponState = EWeaponState::Unequipped;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATimberWeaponBase> CurrentlyEquippedWeapon;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void HandleBuildMenuOpen(bool IsBuildMenuOpen);

	UFUNCTION()
	void PlayDeathAnimation();
};
