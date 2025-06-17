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

	/*Collision Component*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UBoxComponent* WeaponBoxComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* NiagaraEffectSpawnLocation = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* WeaponSkeletalMesh = nullptr;

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
	
	/* Combo */
	UFUNCTION()
	void HandlePlayComboAttackMontage();

	UFUNCTION(BlueprintCallable, Category = "Combo")
	void ResetComboData();
	
	//Initialized to -1 (No Section Played Yet)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	int32 SectionIndex = 0;

	//Whether the player attacked again within the CanCombo window.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	bool bComboInitiated = false;

	//The Window for when additional attack inputs will be allowed to make combo attacks.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	bool bComboWindowOpen = false;

	/* Sounds */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Sounds")
	USoundCue* AttackSwooshSound;

	/*Montage Delegate*/
	//This is a delegate Implemented on AnimInstance (Unreal Engine AnimInstance.h)
	FOnMontageBlendingOutStarted BlendingOutDelegate;
	
	UFUNCTION()
	void HandleAttackMontageInterrupted(UAnimMontage* AnimMontage, bool bArg, bool bCond);
};
