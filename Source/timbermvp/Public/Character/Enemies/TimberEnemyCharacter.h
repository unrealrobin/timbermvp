// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Character/TimberCharacterBase.h"
#include "Interfaces/DamageableEnemy.h"
#include "TimberEnemyCharacter.generated.h"

class ATimberBuildingComponentBase;
class USoundCue;
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
	virtual void TakeDamage(float DamageAmount, AActor* DamageInstigator) override;
	virtual void PlayProjectileHitSound(FHitResult HitResult) override;
	virtual void PlayMeleeWeaponHitSound(FHitResult HitResult) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon State")
	EEnemyWeaponState EnemyWeaponType = EEnemyWeaponState::NoWeaponEquipped;

	float StandardMelleAttackDamage = 50.f;
	float CurrentWave = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	bool bHasBeenAggroByPlayer = false;

	UFUNCTION(BlueprintCallable)
	ATimberBuildingComponentBase* LineTraceToSeeda();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	UAnimMontage* DeathMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	UAnimMontage* StandardAttackMontage;

	//Called from Animation Montage when montage ends.
	UFUNCTION(BlueprintCallable)
	void HandleEnemyDeath();
	void HandleWeaponDestruction();

	/*Hit Sounds*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sounds")
	USoundCue* ProjectileHitSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Sounds")
	USoundCue* MeleeHitSound;

protected:
	virtual void Tick(float DeltaSeconds) override;
	void StopAiControllerBehaviorTree();
	virtual float CalculateOutputDamage(float Damage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Components")
	USceneComponent* RaycastStartPoint;

	/* Death */
	void OnDeath_HandleCollision();
	
	/* Animation */
	void PlayMontageAtRandomSection(UAnimMontage* Montage);

	/*Delegate Functions*/
	UFUNCTION()
	void UpdateCurrentWaveNumber(float CurrentWaveNumber);

	bool HandleAggroCheck(AActor* DamageInstigator, float DamageReceived, float DamageAccumulatedDuringWindow);

	/* Damage effects Aggro */
	FTimerHandle DamageWindowTimerHandle;
	float DamageWindowTime = 3.0f;
	float DamageAccumulatedDuringWindow = 0.f;
	void ResetDamageWindow();
	void StartDamageTimerWindow();
		
};
