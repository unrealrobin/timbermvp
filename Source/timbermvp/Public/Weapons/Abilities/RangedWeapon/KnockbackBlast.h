// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Abilities/WeaponAbilityBase.h"
#include "KnockbackBlast.generated.h"

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UKnockbackBlast : public UWeaponAbilityBase
{
	GENERATED_BODY()

public:
	UKnockbackBlast();

	UPROPERTY()
	UBoxComponent* HitBox = nullptr;

	UFUNCTION()
	void HandleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	virtual void Execute(FAbilityContext Context) override;

protected:

	void MoveBoxForward();
	void ScaleBoxExtent();
	void HandleDestructionCheck();
	void IncrementBlastBox();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knockback Details")
	float ImpulseForce = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knockback Details")
	float KnockbackDuration = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knockback Details")
	FVector InitialBoxExtent = FVector(20.f, 20.f, 20.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knockback Details")
	FVector EndBoxExtent = FVector(20.f, 300.f, 300.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knockback Details")
	float CurrentTimeStep = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knockback Details")
	float EndTimeStep = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knockback Details")
	FVector InitialBoxLocation = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knockback Details")
	float EndLocationDelta = 400.0f;

private:
	FTimerHandle BlastTimerHandle;
};
