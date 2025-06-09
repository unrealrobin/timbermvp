// Property of Paracosm Industries.	

#pragma once

#include "CoreMinimal.h"
#include "Character/TimberCharacterBase.h"
#include "Interfaces/CombatComponentAnimUser.h"
#include "States/PlayerStateBase.h"
#include "Weapons/TimberWeaponBase.h"
#include "TimberPlayableCharacter.generated.h"

class UCombatComponent;
enum class ETutorialState : uint8;
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
enum class ECharacterState: uint8
{
	Standard UMETA(DisplayName = "Standard"),
	Building UMETA(DisplayName = "Building")
};

/**
 * 
 */
UCLASS()
class TIMBERMVP_API ATimberPlayableCharacter : public ATimberCharacterBase, public ICombatComponentAnimUser
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

	/*Interface Overrides*/
	virtual void PlayWeaponEquipAnimationMontage(FName SectionName) override;
	
	//CharacterState
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character State")
	ECharacterState CharacterState = ECharacterState::Standard;

	/* Get Game State */
	ETutorialState GetTutorialState();
	
	/*Components*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	USpringArmComponent* CameraSpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Build System")
	UBuildSystemManagerComponent* BuildSystemManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
	UInventoryManagerComponent* InventoryManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat Component")
	UCombatComponent* CombatComponent;
	
	/*Attributes / Defaults*/
	bool IsRunning = true;
	float MaxRunSpeed = 2000.f;

	/*Animation Montages*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	UAnimMontage* DeathMontage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	UAnimMontage* TutorialWakeMontage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	UAnimMontage* TurnInPlaceMontage;
	
	UFUNCTION()
	void StopAllAnimMontages();
	UFUNCTION()
	void PlayWakeAnimationMontage();

	void PlayAnimationMontageAtSection(UAnimMontage* MontageToPlay, FName SectionName);
	/*Smooth Character Rotation*/

	//Checks if the player is undergoing a Timed Rotation
	bool IsRotating = false;
	//Time Elapsed in-between timer calls.
	float ElapsedTime = 0.0f;
	FTimerHandle RotationTimerHandle;
	//Handles that actual timing and set of the Rotation
	void StartLerpRotation(const FRotator& TargetRotation, float DurationOfRotation);
	
	
	/*Animation Properties*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	bool IsNowJumping = false;
	
	/*Combat*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	UAnimMontage* ReloadMontage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	UAnimMontage* EquipWeaponMontage;
	
	UFUNCTION()
	void PlayEquipWeaponMontage(FName SectionName);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATimberWeaponBase> CurrentlyEquippedWeapon;
	
	void HandleRaycastHitConditions(bool bHits);
	
	ATimberWeaponBase* GetCurrentlyEquippedWeapon() const { return CurrentlyEquippedWeapon; }

	/*Build System*/
	void PerformBuildSystemRaycast();
	
	bool HandleShowDeleteWidget();
	
	UFUNCTION()
	void ResetDeleteIcon();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player Controller")
	ATimberPlayerController* RaycastController;
	
	UPROPERTY(EditAnywhere, Category="Build System Info")
	float BuildRaycastDistance = 2000.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Build System Info")
	bool ShouldRaycast = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Build System Info")
	ABuildableBase* HoveredBuildingComponent;

	UPROPERTY(VisibleAnywhere, Category="Build System Info")
	TArray<FHitResult> HitResults;
	
	FVector BuildingComponentImpactPoint;

	/*Damage*/
	void PlayerTakeDamage(float DamageAmount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	float DamageModifierValue = 1.f;

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
	
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void HandleBuildMenuOpen(bool IsBuildMenuOpen);

	UFUNCTION()
	void PlayDeathAnimation();
	
};

