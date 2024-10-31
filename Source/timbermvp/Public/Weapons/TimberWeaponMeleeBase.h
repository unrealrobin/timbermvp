// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/TimberWeaponBase.h"
#include "TimberWeaponMeleeBase.generated.h"

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

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*Collision Component*/
	UPROPERTY(EditAnywhere, Category="Components")
	UBoxComponent* WeaponBoxComponent;
	
	UFUNCTION(BlueprintCallable, Category="Collision")
	void HandleWeaponCollision(bool ShouldReadyCollision) const;
	
	virtual void OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* 
	OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) ;
	
	virtual void OnWeaponOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* 
	OtherComp, int32 OtherBodyIndex);

	/* Attack */
	UFUNCTION()
	void HandlePlayAttackMontage() const;

	UFUNCTION(BlueprintCallable, Category="Weapons")
	virtual void PerformStandardAttack();
};
