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

public:

	UPROPERTY(EditAnywhere, Category="Weapon Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Components")
	USceneComponent* ProjectileSpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Components")
	TSubclassOf<ATimberProjectileBase> ProjectileType;

	UFUNCTION()
	float GetWeaponBaseDamage() const {return BaseWeaponDamage;}

	UFUNCTION()
	void FireRangedWeapon();

	UPROPERTY(VisibleAnywhere, Category="Attack Info")
	TArray<AActor*> ActorsToIgnore;
};
