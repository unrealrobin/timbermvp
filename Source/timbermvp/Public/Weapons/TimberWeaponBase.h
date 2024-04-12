// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimberWeaponBase.generated.h"

class UCapsuleComponent;

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

	UPROPERTY(BlueprintReadWrite, Category="Weapon Details")
	float BaseWeaponDamage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation Montage")
	UAnimMontage* AttackMontage;

public:

	/*Anim Montage Section Names*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation")
	FName StandardAttack1 = "Standard_Horizontal";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation")
	FName StandardAttack2 = "Standard_Downward";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation")
	FName HeavyCombo = "Heavy_Combo";

	UPROPERTY(EditAnywhere, Category="Weapon Components")
	UCapsuleComponent* CapsuleComponent;
	UPROPERTY(EditAnywhere, Category="Weapon Components")
	UStaticMeshComponent* StaticMesh;

	UFUNCTION()
	void HandlePlayAttackMontage() const;
};
