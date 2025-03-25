// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "TrapBase.h"
#include "SpikeTrap.generated.h"

class UCurveVector;
class UTimelineComponent;

UCLASS()
class TIMBERMVP_API ASpikeTrap : public ATrapBase
{
	GENERATED_BODY()

public:
	ASpikeTrap();
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	void SetSpikeOutAttackTimer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trap Component")
	UStaticMeshComponent* TrapSpikeMesh;

	UFUNCTION()
	void HandleSpikeTrapOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void HandleSpikeOutAttack();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	float SpikeDamage = 35.f;

	void ApplyDamageToActorsInHitBox();

	/*Timeline Animation*/
	void SetupTimelineComponents();
	void SetupTimelineData();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UTimelineComponent* SpikeOutTimeline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UCurveVector* SpikeAnimVectorCurve;
	UFUNCTION()
	void HandleVectorCurveUpdate(FVector CurveVector);
	UFUNCTION()
	void SpikeOutTimelineFinished();
	UFUNCTION()
	void PlaySpikeOutTimeline();
	UFUNCTION()
	void PlaySpikeOutTimeline_Reverse();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	bool IsSpikesOut = false;

	/*Timers*/
	FTimerHandle TimeToActiveSpikeOutAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float TimeToActiveSpikeOutAttackValue = 0.3f;
	FTimerHandle SpikeOutCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float SpikeOutCooldownValue = 3.0f;
	bool IsSpikeOnCooldown = false;
	void EndSpikeTrapCooldown(); // Empty function to be used in the timer manager.
};
