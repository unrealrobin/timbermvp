// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimberWeaponBase.generated.h"

class ATimberProjectileBase;
class UBoxComponent;

UCLASS()
class TIMBERMVP_API ATimberWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimberWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Details")
	float BaseWeaponDamage = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation Montage")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Components")
	USceneComponent* TraceBoxStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Components")
	USceneComponent* TraceBoxEnd;

	UPROPERTY(BlueprintReadOnly, Category="Weapon Details")
	TArray<AActor*> ActorsToIgnore;

	UFUNCTION(Category="Weapon")
	void ApplyDamageOnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult);

	

public:

	/*Anim Montage Section Names*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation")
	FName StandardAttack1 = "Standard_1";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation")
	FName StandardAttack2 = "Standard_2";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation")
	FName SpecialAttack = "Special_Attack";

	UPROPERTY(EditAnywhere, Category="Weapon Components")
	UBoxComponent* WeaponBoxComponent;
	UPROPERTY(EditAnywhere, Category="Weapon Components")
	UStaticMeshComponent* StaticMesh;

	UFUNCTION()
	void HandlePlayAttackMontage() const;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void ReadyWeaponCollision(bool ShouldReadyCollision) const;

	UFUNCTION()
	void OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnWeaponOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(Category="Weapon")
	void PerformStandardAttack();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Components")
	USceneComponent* ProjectileSpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Components")
	TSubclassOf<ATimberProjectileBase> ProjectileType;

	UFUNCTION()
	float GetWeaponBaseDamage() const {return BaseWeaponDamage;}

	UFUNCTION()
	void FireRangedWeapon();
};
