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
	
	UFUNCTION()
	virtual float GetWeaponBaseDamage() const {return BaseWeaponDamage;}

	UPROPERTY(VisibleAnywhere, Category="Attack Info")
	TArray<AActor*> ActorsToIgnore;
};
