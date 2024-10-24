// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Character/TimberCharacterBase.h"
#include "Interfaces/DamageableEnemy.h"
#include "TimberEnemyCharacter.generated.h"

class ATimberBuildingComponentBase;

UENUM(BlueprintType)
enum class EEnemyWeaponState : uint8
{
	NoWeaponEquipped UMETA(DisplayName = "NoWeaponEquipped"),
	MeleeWeaponEquipped UMETA(DisplayName = "MeleeWeaponEquipped"),
	RangedWeaponEquipped UMETA(DisplayName = "RangedWeaponEquipped"),
};

/**
 * 
 */
UCLASS()
class TIMBERMVP_API ATimberEnemyCharacter : public ATimberCharacterBase, public IDamageableEnemy
{
	GENERATED_BODY()
	
public:
	ATimberEnemyCharacter();
	
	virtual void BeginPlay() override;
	virtual void TakeDamage(float DamageAmount) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon State")
	EEnemyWeaponState EnemyWeaponType = EEnemyWeaponState::NoWeaponEquipped;

	float StandardMelleAttackDamage = 50.f;
	float CurrentWave = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Combat")
	bool bHasBeenAggroByPlayer = false;

	UFUNCTION(BlueprintCallable)
	ATimberBuildingComponentBase* LineTraceToSeeda();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	UAnimMontage* DeathMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	UAnimMontage* StandardAttackMontage;
protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual float CalculateOutputDamage(float Damage);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Components")
	USceneComponent* RaycastStartPoint;
	
	/*Delegate Functions*/
	UFUNCTION()
	void UpdateCurrentWaveNumber(float CurrentWaveNumber);
};
