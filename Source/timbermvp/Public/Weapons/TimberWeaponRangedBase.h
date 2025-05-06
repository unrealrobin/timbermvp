// Property of Paracosm Industries.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/TimberWeaponBase.h"
#include "TimberWeaponRangedBase.generated.h"

class ATimberCharacterBase;
class USoundCue;

UCLASS()
class TIMBERMVP_API ATimberWeaponRangedBase : public ATimberWeaponBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATimberWeaponRangedBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ammo")
	int CurrentAmmo = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int MaxAmmo = 20;
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

	/*
	 * Unit offset from Direct Center in All Axis
	 * 1 is 100% Accuracy
	 * Ex. 5 would be +5 and -5 in all axis, and a Random Number taken from that range.
	 * Large is less accurate, small is more accurate.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Accuract")
	int WeaponAccuracy = 20;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Components")
	USceneComponent* ProjectileSpawnComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Components")
	TSubclassOf<ATimberProjectileBase> ProjectileType;

	UFUNCTION()
	void FireRangedWeapon(FVector TargetLocation);

	UFUNCTION(BlueprintCallable)
	void AI_FireRangedWeapon();
	
	/*Reloading*/
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon")
	bool bIsReloading = false;

	UFUNCTION()
	void PlayReloadMontage();

	UFUNCTION()
	void HandleReloadMontageInterruption(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void ReloadWeapon();

	/*Sound*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Sounds")
	USoundCue* FiringSound;
};
