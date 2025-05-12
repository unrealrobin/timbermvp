// Property of Paracosm Industries.

#pragma once

#include "CoreMinimal.h"
#include "Character/TimberCharacterBase.h"
#include "Interfaces/DamageableEnemy.h"
#include "TimberEnemyCharacter.generated.h"

class ULootTable;
class ALootHealthDrop;
class AEnemyLootDropBase;
class ATimberBuildingComponentBase;
class USoundCue;

UENUM(BlueprintType)
enum class EEnemyWeaponState : uint8
{
	NoWeaponEquipped UMETA(DisplayName = "NoWeaponEquipped"),
	MeleeWeaponEquipped UMETA(DisplayName = "MeleeWeaponEquipped"),
	RangedWeaponEquipped UMETA(DisplayName = "RangedWeaponEquipped"),
};

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	BasicRobot UMETA(DisplayName = "BasicRobot"),
	MeleeWeaponRobot UMETA(DisplayName = "MeleeWeaponRobot"),
	RangedWeaponRobot UMETA(DisplayName = "RangedWeaponRobot"),
	Default UMETA(DisplayName = "Default")
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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TakeDamage(float DamageAmount, AActor* DamageInstigator) override;
	
	virtual void PlayProjectileHitSound(FHitResult HitResult) override;
	virtual void PlayMeleeWeaponHitSound(FHitResult HitResult) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon State")
	EEnemyWeaponState EnemyWeaponType = EEnemyWeaponState::NoWeaponEquipped;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Type")
	EEnemyType EnemyType = EEnemyType::Default;
	
	float StandardMelleAttackDamage = 50.f;
	float CurrentWave = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	bool bHasBeenAggroByPlayer = false;
	
	/* Montages */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	UAnimMontage* DeathMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	UAnimMontage* StandardAttackMontage;

	/*
	 * Sections:
	 * @param:Front
	 * @param:Back
	 * @param:Left
	 * @param:Right
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	UAnimMontage* HitReactMontage;

	//Called from Animation Montage when montage ends.
	UFUNCTION(BlueprintCallable)
	void HandleEnemyDeath();
	
	void HandleWeaponDestruction();

	/*Hit Sounds*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sounds")
	USoundCue* ProjectileHitSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Sounds")
	USoundCue* MeleeHitSound;

	/*Sweep / Traces */
	UFUNCTION(BlueprintCallable)
	void SweepForActor(TSubclassOf<AActor> ActorToSweepFor, float SphereRadius = 100.0f, float DistanceToCheck = 100.0f);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sweep")
	AActor* SweepHitActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sweep")
	float SweepDistance = 250.f;

	UFUNCTION(BlueprintCallable)
	ATimberBuildingComponentBase* LineTraceToSeeda();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Target")
	AActor* CurrentTarget = nullptr;

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

	/* Loot */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	ULootTable* LootTable = nullptr;
	
	void OnDeath_DropLoot();
	
	/*void SpawnLoot(TSubclassOf<AEnemyLootDropBase> LootDropClass);*/

	/*virtual void HandleDropHealthLoot(TSubclassOf<AEnemyLootDropBase> HealthDropClass);

	//Basic Health Drop Chance - If in Loot Array
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Loot")
	float HealthDropChance = 20.f;
	
	//Max Health Drop Chance - If in Loot Array
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Loot")
	float MaxHealthDropChance = 5.f;
	
	// Items that can be dropped by any enemy.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Loot")
	TArray<TSubclassOf<AEnemyLootDropBase>> StandardLootArray;
	
	//Specific Enemies may drop specific Loot Items.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Loot")
	TArray<TSubclassOf<AEnemyLootDropBase>> UniqueLootArray;*/

	
};
