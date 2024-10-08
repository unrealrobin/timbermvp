// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Character/TimberCharacterBase.h"
#include "Interfaces/DamageableEnemy.h"
#include "TimberEnemyCharacter.generated.h"

class ATimberBuildingComponentBase;
/**
 * 
 */
UCLASS()
class TIMBERMVP_API ATimberEnemyCharacter : public ATimberCharacterBase, public IDamageableEnemy
{
	GENERATED_BODY()

	ATimberEnemyCharacter();

public:
	virtual void BeginPlay() override;

	float StandardMelleAttackDamage = 50.f;
	float CurrentWave = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Combat")
	bool bHasBeenAggroByPlayer = false;

	UFUNCTION(BlueprintCallable)
	ATimberBuildingComponentBase* LineTraceToSeeda();

protected:
	virtual void Tick(float DeltaSeconds) override;

	virtual void TakeDamage(float DamageAmount) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAnimMontage* StandardMeleeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Components")
	UCapsuleComponent* KickCollisionSphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Components")
	USceneComponent* RaycastStartPoint;

	UFUNCTION(BlueprintCallable)
	void HandleKickOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION(BlueprintCallable)
	void DisableKickCollision();
	UFUNCTION(BlueprintCallable)
	void EnableKickCollision();

	/*Delegate Functions*/
	UFUNCTION()
	void UpdateCurrentWaveNumber(float CurrentWaveNumber);
};
