// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/TimberWeaponBase.h"
#include "TimberWeaponRangedBase.generated.h"

UCLASS()
class TIMBERMVP_API ATimberWeaponRangedBase : public ATimberWeaponBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATimberWeaponRangedBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Components")
	USceneComponent* ProjectileSpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Components")
	TSubclassOf<ATimberProjectileBase> ProjectileType;

	UFUNCTION()
	float GetWeaponBaseDamage() const {return BaseWeaponDamage;}

	UFUNCTION()
	void FireRangedWeapon();

	UFUNCTION(Category="Weapon")
	void PerformStandardAttack();*/
};
