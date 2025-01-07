// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/TimberWeaponBase.h"
#include "TimberWeaponRangedBase.generated.h"

class ATimberCharacterBase;

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

public:

	UPROPERTY(VisibleAnywhere, Category = Owner)
	AActor* WeaponOwner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Components")
	USceneComponent* ProjectileSpawnComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Components")
	TSubclassOf<ATimberProjectileBase> ProjectileType;
	
	virtual float GetWeaponBaseDamage() const override {return BaseWeaponDamage;} 

	UFUNCTION()
	void FireRangedWeapon(FVector TargetLocation);

	UFUNCTION(BlueprintCallable)
	void AI_FireRangedWeapon();

	UFUNCTION(Category="Weapon")
	void PerformStandardAttack();

	/*Sound*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Sounds")
	USoundCue* FiringSound;
};
