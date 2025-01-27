// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Character/TimberCharacterBase.h"
#include "States/PlayerStateBase.h"
#include "Weapons/TimberWeaponBase.h"
#include "TimberPlayableCharacter.generated.h"

class ABuildableBase;
class UInventoryObject;
class ATimberPlayerController;
class ATimberWeaponRangedBase;
class ATimberWeaponMeleeBase;
class UBuildSystemManagerComponent;
class ATimberBuildingComponentBase;
class USpringArmComponent;
class UCameraComponent;
class UInventoryManagerComponent;

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
	ATimberPlayableCharacter();

	//Delegates Signatures
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterInitialization);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHandlePlayerDeath, bool, bIsPlayerDead);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHandleSpawnDeleteIconLocation, float, ViewportLocationX, float, ViewportLocationY);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHandleRemoveDeleteIcon);
	
	//Delegate Handles
	FOnCharacterInitialization OnCharacterInitialization;
	FHandlePlayerDeath HandlePlayerDeath_DelegateHandle;
	FHandleSpawnDeleteIconLocation HandleSpawnDeleteIconLocation_DelegateHandle;
	FHandleRemoveDeleteIcon HandleRemoveDeleteIcon_DelegateHandle;
	

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
	UInventoryManagerComponent* InventoryManager;

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
	bool HandleShowDeleteWidget();
	void HandleRaycastHitConditions(bool bHits);
	ATimberWeaponBase* GetCurrentlyEquippedWeapon() const { return CurrentlyEquippedWeapon; }

	/*Build System*/
	void PerformBuildSystemRaycast();

	UFUNCTION()
	void ResetDeleteIcon();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player Controller")
	ATimberPlayerController* RaycastController;
	
	UPROPERTY(EditAnywhere, Category="Build System Info")
	float BuildRaycastDistance = 1000.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Build System Info")
	bool ShouldRaycast = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Build System Info")
	ABuildableBase* HoveredBuildingComponent;

	UPROPERTY(VisibleAnywhere, Category="Build System Info")
	TArray<FHitResult> HitResults;
	
	FVector BuildingComponentImpactPoint;

	/*Damage*/
	void PlayerTakeDamage(float DamageAmount);

	/*Death*/
	UFUNCTION()
	void HandlePlayerDeath(bool bIsPlayerDead = false);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health")
	bool bIsPlayerDead = false;

	/*Inventory*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
	UInventoryObject* InventoryObject = nullptr;

	UFUNCTION()
	void GetPlayerInventoryFromPlayerState();

	UFUNCTION()
	void UnbindFromSeedaDeathDelegate(AActor* DestroyedActor);
	
	UFUNCTION()
	void BindToSeedaDelegates(AActor* Seeda);

protected:
	virtual void BeginPlay() override;
	
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
