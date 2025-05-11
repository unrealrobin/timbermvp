// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "Weapons/TimberWeaponBase.h"
#include "CombatComponent.generated.h"

enum class EAbilityInputRequirement : uint8;
class UCombatComponent;
struct FInputActionValue;

UENUM(BlueprintType)
enum class EOwnerWeaponState : uint8
{
	Unequipped UMETA(DisplayName = "None"),
	MeleeWeaponEquipped UMETA(DisplayName = "MeleeWeaponEquipped"),
	RangedWeaponEquipped UMETA(DisplayName = "RangedEquipped"),
	Default UMETA(DisplayName = "Default")
};

USTRUCT(BlueprintType)
struct FAbilityContext
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability Context")
	UCombatComponent* CombatComponent = nullptr;

	//The player/Character that initiated the ability
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability Context")
	AActor* Instigator = nullptr;

	//The Weapon that initiated the ability
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability Context")
	ATimberWeaponBase* WeaponInstance = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability Context")
	FVector TargetLocation = FVector::ZeroVector;
	
	FInputActionValue InputActionValue = FInputActionValue();
};

class ATimberPlayableCharacter;
class ATimberWeaponRangedBase;
class ATimberWeaponMeleeBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TIMBERMVP_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UCombatComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Data")
	ACharacter* OwningCharacter = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Data")
	ATimberWeaponBase* CurrentlyEquippedWeapon = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Data")
	EOwnerWeaponState CurrentWeaponState = EOwnerWeaponState::Unequipped;
	
	/* Weapon Classes*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	TSubclassOf<ATimberWeaponBase> RangedWeaponClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	TSubclassOf<ATimberWeaponBase> MeleeWeaponClass;
	
	void EquipWeapon(ATimberWeaponBase* WeaponInstance, FName EquippedWeaponSocketName);
	void UnEquipWeapon(ATimberWeaponBase* WeaponInstance, FName UnEquipSocketName);

	FAbilityContext GenerateCurrentAbilityContext(const FInputActionValue& InputValue);

	bool ValidateNoResourceCostAbility(const UWeaponAbilityBase* WeaponAbilityBase);
	bool ValidateWeaponAbility(const UWeaponAbilityBase* AbilityToValidate);

public:

	/*Weapon Instances*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	ATimberWeaponRangedBase* RangedWeaponInstance = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	ATimberWeaponMeleeBase* MeleeWeaponInstance = nullptr;
	

	UFUNCTION(BlueprintCallable)
	void EquipMelee();
	UFUNCTION(BlueprintCallable)
	void EquipRanged();
	
	void SpawnRangedWeapon();
	void SpawnMeleeWeapon();
	void UnEquipAllWeapons();
	void UnEquipCurrentlyEquippedWeapon();
	void UpdateCurrentWeaponState(EOwnerWeaponState NewWeaponState);
	void ReloadRangedWeapon();
	void HandlePrimaryAbility(const FInputActionValue& Value);
	void HandleSecondaryAbility(const FInputActionValue& Value);
	void HandleSecondaryAbility_Started(const FInputActionValue& Value);
	void HandleSecondaryAbility_Cancelled(const FInputActionValue& Value);
	void HandleSecondaryAbility_Completed(const FInputActionValue& Value);

	bool bIsEquipMontagePlaying = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanMeleeAttack = true;
	
	void PlayEquipWeaponMontage(FName MontageSectionName);

	UFUNCTION()
	void HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	//If tracked, Montage Stages must be tracked on the Current Ability.
	void PlayCharacterAnimationMontage(UAnimMontage* Montage, FName MontageSectionName, float PlayRate = 1.0f, bool TrackStages = false);

	void StopCharacterAnimationMontage(UAnimMontage* Montage, float BlendOutTime);
	
	/*Socket Names Set on SKM of Kip. Can be Overwritten.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Sockets")
	FName UnEquippedMeleeSocket = "UnEquippedMeleeSocket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Sockets")
	FName EquippedMeleeSocket = "EquippedMeleeSocket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Sockets")
	FName UnEquippedRangeSocket = "UnEquippedRangeSocket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Sockets")
	FName EquippedRangeSocket = "EquippedRangeSocket";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Targeting")
	float ProjectileAlignmentAdjustmentDistance = 500.0f;

	float ControllerInputStartTime = 0;
	float ControllerInputEndTimer = 0;

	EAbilityInputRequirement GetAbilityInputRequirement(bool bIsPrimaryAbility) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability Context")
	UWeaponAbilityBase* CurrentWeaponAbility = nullptr;

	FORCEINLINE ATimberWeaponBase* GetCurrentlyEquippedWeapon() const { return CurrentlyEquippedWeapon; }
	FORCEINLINE EOwnerWeaponState GetCurrentWeaponState() const { return CurrentWeaponState; }
	
private:
	
	UFUNCTION()
	void SpawnWeaponAtSocketLocation(TSubclassOf<ATimberWeaponBase> WeaponClassToSpawn, FName SocketName);

	FVector GetProjectileTargetLocation();
	
	UPROPERTY()
	bool bIsRifleEquipped = false;
	
	UPROPERTY()
	bool bIsMeleeEquipped = false;
	
	UFUNCTION()
	void SendWeaponStateToOwnerAnimInstance();

	bool bHasEnoughPower(float AbilityCost, float CurrentWeaponPower);

	void ConsumePower(ATimberWeaponBase* WeaponInstance, float AmountToConsume);

	bool ValidatePowerWeaponAbility(const UWeaponAbilityBase* AbilityToValidate);
	
};
