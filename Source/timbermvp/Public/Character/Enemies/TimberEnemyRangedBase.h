// Property of Paracosm Industries.

#pragma once

#include "CoreMinimal.h"
#include "TimberEnemyCharacter.h"
#include "TimberEnemyRangedBase.generated.h"

class ATimberWeaponRangedBase;
class ATimberWeaponBase;

UCLASS()
class TIMBERMVP_API ATimberEnemyRangedBase : public ATimberEnemyCharacter
{
	GENERATED_BODY()

public:
	ATimberEnemyRangedBase();
	
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void EquipRangedWeapon();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	TSubclassOf<ATimberWeaponBase> RangedWeaponClassName = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	ATimberWeaponRangedBase* EquippedWeapon = nullptr;


	//Assists in Aim Offset Calculations in Animation Blueprint
	UFUNCTION(BlueprintCallable)
	void GetRotationToCurrentTarget();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="RangedTargetRotation")
	float PitchToTarget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="RangedTargetRotation")
	float YawToTarget;
};
