// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "TimberEnemyCharacter.h"
#include "TimberEnemyRangedBase.generated.h"

class ATimberWeaponBase;

UCLASS()
class TIMBERMVP_API ATimberEnemyRangedBase : public ATimberEnemyCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATimberEnemyRangedBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	ATimberWeaponBase* RangedWeapon;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
