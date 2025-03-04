// Property of Paracosm Industries. Dont use my shit.

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
	// Called every frames
	virtual void Tick(float DeltaTime) override;

	/*Collision Component*/
	UPROPERTY(EditAnywhere, Category="Components")
	UBoxComponent* WeaponBoxComponent;

	UFUNCTION(BlueprintCallable, Category="Collision")
	void HandleWeaponCollision(bool ShouldReadyCollision) const;

	UFUNCTION()
	virtual void OnWeaponOverlapBegin(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent*
		OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnWeaponOverlapEnd(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent*
		OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void EmptyActorToIgnoreArray();

	/* Attack */
	UFUNCTION()
	void HandlePlayAttackMontage();
	UFUNCTION(BlueprintCallable, Category="Weapons")
	virtual void PerformStandardAttack();

	/* Sounds */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Sounds")
	USoundCue* AttackSwooshSound;

	/*Montage Delegate*/
	//Used to make sure that attack animations that are interrupted reset the CanAttackAgain bool on the Controller.
	FOnMontageBlendingOutStarted BlendingOutDelegate;
	
	UFUNCTION()
	void HandleAttackMontageInterrupted(UAnimMontage* AnimMontage, bool bArg, bool bCond);
};
