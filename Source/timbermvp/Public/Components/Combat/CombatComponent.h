// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/TimberWeaponBase.h"
#include "CombatComponent.generated.h"

UENUM(BlueprintType)
enum class EOwnerWeaponState : uint8
{
	Unequipped UMETA(DisplayName = "None"),
	MeleeWeaponEquipped UMETA(DisplayName = "MeleeWeaponEquipped"),
	RangedWeaponEquipped UMETA(DisplayName = "RangedEquipped"),
	Default UMETA(DisplayName = "Default")
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
	
	/*Weapon Instances*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	ATimberWeaponRangedBase* RangedWeaponInstance = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	ATimberWeaponMeleeBase* MeleeWeaponInstance = nullptr;
	
	void EquipWeapon(ATimberWeaponBase* WeaponInstance, FName EquippedWeaponSocketName);
	void UnEquipWeapon(ATimberWeaponBase* WeaponInstance, FName UnEquipSocketName);
	

public:

	UFUNCTION(BlueprintCallable)
	void EquipMelee();
	
	UFUNCTION(BlueprintCallable)
	void EquipRanged();
	
	void SpawnRangedWeapon();
	void SpawnMeleeWeapon();
	void UnEquipAllWeapons();
	void UpdateCurrentWeaponState(EOwnerWeaponState NewWeaponState);
	
	/*Socket Names Set on SKM of Kip. Can be Overwritten.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Sockets")
	FName UnEquippedMeleeSocket = "UnEquippedMeleeSocket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Sockets")
	FName EquippedMeleeSocket = "EquippedMeleeSocket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Sockets")
	FName UnEquippedRangeSocket = "UnEquippedRangeSocket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Sockets")
	FName EquippedRangeSocket = "EquippedRangeSocket";

	FORCEINLINE ATimberWeaponBase* GetCurrentlyEquippedWeapon() const { return CurrentlyEquippedWeapon; }
	
private:
	
	UFUNCTION()
	void SpawnWeaponAtSocketLocation(TSubclassOf<ATimberWeaponBase> WeaponClassToSpawn, FName SocketName);

	UPROPERTY()
	bool bIsRifleEquipped = false;
	UPROPERTY()
	bool bIsMeleeEquipped = false;
	
	UFUNCTION()
	void SendWeaponStateToOwnerAnimInstance();
	
};
