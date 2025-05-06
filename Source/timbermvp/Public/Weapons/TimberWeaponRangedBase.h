// Property of Paracosm Industries.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/CombatComponent.h"
#include "Weapons/TimberWeaponBase.h"
#include "TimberWeaponRangedBase.generated.h"

class ATimberCharacterBase;
class USoundCue;

UENUM(BlueprintType)
enum class EAbilityType : uint8
{
	None,
	BasicProjectile,
	Knockback
};

//Declerations for Struct
class ATimberWeaponRangedBase;

USTRUCT(BlueprintType)
struct FRangedAbilityData
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AbilityName = FName("None");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAbilityType AbilityType = EAbilityType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AbilityPowerCost = 0.0f;

	void Execute(FAbilityContext Context) const;
	
};

UCLASS()
class TIMBERMVP_API ATimberWeaponRangedBase : public ATimberWeaponBase
{
	GENERATED_BODY()

public:
	ATimberWeaponRangedBase();
	
	virtual void Tick(float DeltaTime) override;

	/*Sound*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Sounds")
	USoundCue* FiringSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Components")
	USceneComponent* ProjectileSpawnComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Components")
	TSubclassOf<ATimberProjectileBase> ProjectileType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	FTimerHandle TimeBetweenShotsHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float TimeBetweenProjectiles = 0.1f;

	//Default false, shot turns to true, timer turns back to false.
	bool bIsFireOnCooldown = false;

	UFUNCTION()
	void ResetFiringCooldown();

public:
	UPROPERTY(VisibleAnywhere, Category = Owner)
	AActor* WeaponOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ammo")
	int CurrentAmmo = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int MaxAmmo = 20;
	
	/*
	 * Unit offset from Direct Center in All Axis
	 * 1 is 100% Accuracy
	 * Ex. 5 would be +5 and -5 in all axes, and a Random Number taken from that range.
	 * Large is less accurate, small is more accurate.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Accuract")
	int WeaponAccuracy = 20;
	
	UFUNCTION()
	void FireRangedWeapon(FVector TargetLocation);

	UFUNCTION(BlueprintCallable)
	void AI_FireRangedWeapon();
	
	/*Reloading*/
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon")
	bool bIsReloading = false;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void ReloadWeapon();

	UFUNCTION()
	void PlayReloadMontage();

	UFUNCTION()
	void HandleReloadMontageInterruption(UAnimMontage* Montage, bool bInterrupted);

	/*Abilities & Ability Logic*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities", meta=(TitleProperty="AbilityName") )
	TArray<FRangedAbilityData> RangedAbilitiesArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	FRangedAbilityData PrimaryAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	FRangedAbilityData SecondaryAbility;

	void Execute_BasicProjectile(FAbilityContext Context);
	
	void Execute_Knockback(FAbilityContext Context);
};
