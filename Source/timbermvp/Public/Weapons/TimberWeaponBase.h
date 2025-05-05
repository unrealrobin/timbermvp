// Property of Paracosm Industries.

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

	//Used for Sword Only. Ranged Damage based on Projectile + Modifiers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Details")
	float BaseWeaponDamage = 35.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation Montage")
	UAnimMontage* AttackMontage;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageModifierValue = 1.0f;
	
	//TODO:: Might be adjusted later to account for Enemy Specific Modifiers aswell.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Details")
	float TotalWeaponDamage = BaseWeaponDamage * DamageModifierValue;
	
	UPROPERTY(EditAnywhere, Category="Weapon Components")
	UStaticMeshComponent* StaticMesh;

	/*UFUNCTION()
	virtual float GetWeaponBaseDamage() const { return BaseWeaponDamage; }*/

	UPROPERTY(VisibleAnywhere, Category="Attack Info")
	TArray<AActor*> ActorsToIgnore;
};
