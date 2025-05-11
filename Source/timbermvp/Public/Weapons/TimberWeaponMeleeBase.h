// Property of Paracosm Industries.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/TimberWeaponBase.h"
#include "TimberWeaponMeleeBase.generated.h"

class ATimberPlayableCharacter;
class USoundCue;

UCLASS()
class TIMBERMVP_API ATimberWeaponMeleeBase : public ATimberWeaponBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATimberWeaponMeleeBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	AActor* WeaponOwner;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	ATimberPlayableCharacter* WeaponInstigator;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapons")
	float MaxWeaponEnergy = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapons")
	float CurrentWeaponEnergy = 100.0f;

	/*Collision Component*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UBoxComponent* WeaponBoxComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* NiagaraEffectSpawnLocation = nullptr;

	UFUNCTION(BlueprintCallable, Category="Collision")
	void HandleWeaponCollision(bool ShouldReadyCollision) const;
	
	UFUNCTION()
	void OnWeaponOverlapBegin(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent*
		OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnWeaponOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	

	UFUNCTION()
	void OnPlayerWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent*
		OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void EmptyActorToIgnoreArray();
	
	UFUNCTION()
	virtual void OnAiWeaponOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent*
		OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/* Attack */
	UFUNCTION()
	void HandlePlayAttackMontage();

	/*Animation*/
	/*UFUNCTION(BlueprintCallable, Category="Weapons")
	virtual void PerformStandardAttack();
	*/

	/* Sounds */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Sounds")
	USoundCue* AttackSwooshSound;

	/*Montage Delegate*/
	//This is a delegate Implemented on AnimInstance (Unreal Engine AnimInstance.h)
	FOnMontageBlendingOutStarted BlendingOutDelegate;
	
	UFUNCTION()
	void HandleAttackMontageInterrupted(UAnimMontage* AnimMontage, bool bArg, bool bCond);
};
