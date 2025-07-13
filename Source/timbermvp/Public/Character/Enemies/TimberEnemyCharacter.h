// Property of Paracosm Industries.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "BuildSystem/BuildableBase.h"
#include "Character/TimberCharacterBase.h"
#include "Interfaces/DamageableEnemy.h"
#include "TimberEnemyCharacter.generated.h"

class AFloatingDataContainer;
class UMetaSoundSource;
class UNavigationHelperComponent;
class UWidgetComponent;
class UStatusEffectHandlerComponent;
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
	Drones UMETA(DisplayName = "Drones"),
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
	
	UFUNCTION()
	void HandleOnMovementModeChanged(class ACharacter* Character, EMovementMode PrevMovementMode, uint8 PreviousCustomMode);
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void TakeDamage(FDamagePayload DamagePayload) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Damage)
	UStatusEffectHandlerComponent* StatusEffectHandler;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Navigation")
	UNavigationHelperComponent* NavHelperComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status Effects")
	UWidgetComponent* StatusEffectBarComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Movement")
	UCharacterMovementComponent* CharacterMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Effects")
	USceneComponent* DamageEffectUISpawnPoint;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon State")
	EEnemyWeaponState EnemyWeaponType = EEnemyWeaponState::NoWeaponEquipped;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Type")
	EEnemyType EnemyType = EEnemyType::Default;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sounds")
	UMetaSoundSource* ProjectileHitSound_MetaSound;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Sounds")
	UMetaSoundSource* MeleeHitSound_MetaSound;
	
	virtual void PlayProjectileHitSound(FHitResult HitResult) override;
	virtual void PlayMeleeWeaponHitSound(FHitResult HitResult) override;
	
	void SetupCharacterMovementData();
	
	/*Base Data*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base Data")
	float MaxWalkSpeedBase = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base Data")
	float StandardMelleAttackDamage = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	bool bHasBeenAggroByPlayer = false;
	
	/* Montages */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	UAnimMontage* DeathMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	UAnimMontage* StandardAttackMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	UAnimMontage* SelfDestructMontage;
	
	UFUNCTION(BlueprintCallable)
	void HandleEnemyDeath();
	
	void HandleWeaponDestruction();
	
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

	UFUNCTION()
	void SelfDestruct();

	void SpawnEffectNameUI(FName EffectName);
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TSubclassOf<AFloatingDataContainer> FloatingDamageContainerClass;
	
	void SpawnDamageUI(FDamagePayload DamagePayload);
	
	virtual void Tick(float DeltaSeconds) override;
	
	void StopAiControllerBehaviorTree();
	
	virtual float CalculateOutputDamage(float Damage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Components")
	USceneComponent* RaycastStartPoint;

	/* Death */
	void OnDeath_HandleCollision();
	
	/* Animation */
	void PlayMontageAtRandomSection(UAnimMontage* Montage);

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

private:

	void ScaleHealth();
	
	void HandleRemoveStatusEffectComponent();

	UFUNCTION()
	void HandleToggleDataView(FInputActionValue Input);

	void SetupStatusEffectBar();
	
	UFUNCTION()
	void HandleOnLanded(const FHitResult& Hit);
	
	void SetupCharacterMovementDelegates();
};
