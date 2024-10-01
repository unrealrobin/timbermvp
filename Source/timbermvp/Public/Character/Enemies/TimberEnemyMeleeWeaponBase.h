// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "TimberEnemyMeleeBase.h"
#include "TimberEnemyMeleeWeaponBase.generated.h"

class ATimberWeaponBase;

UCLASS()
class TIMBERMVP_API ATimberEnemyMeleeWeaponBase : public ATimberEnemyMeleeBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATimberEnemyMeleeWeaponBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Info")
	ATimberWeaponBase* EquippedWeapon;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
