// Property of Paracosm Industries. Dont use my shit.

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ammo")
	int CurrentAmmo = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int MaxAmmo = 30;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Components")
	USceneComponent* ProjectileSpawnComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Components")
	TSubclassOf<ATimberProjectileBase> ProjectileType;

	UFUNCTION()
	void FireRangedWeapon(FVector TargetLocation);

	UFUNCTION(BlueprintCallable)
	void AI_FireRangedWeapon();

	UFUNCTION(Category="Weapon")
	void PerformStandardAttack();

	UFUNCTION()
	void PlayReloadMontage();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void ReloadWeapon();

	/*Sound*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Sounds")
	USoundCue* FiringSound;
};
