// Property of Paracosm Industries. Dont use my shit.

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ".Weapon Info")
	TSubclassOf<ATimberWeaponBase> RangedWeaponClassName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ".Weapon Info")
	ATimberWeaponRangedBase* EquippedWeapon;

	
	
};
