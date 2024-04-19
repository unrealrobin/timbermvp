// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Character/TimberCharacterBase.h"
#include "Interfaces/DamageableEnemy.h"
#include "TimberEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TIMBERMVP_API ATimberEnemyCharacter : public ATimberCharacterBase, public IDamageableEnemy
{
	GENERATED_BODY()

	ATimberEnemyCharacter();

public:
	virtual void BeginPlay() override;


protected:
	virtual void Tick(float DeltaSeconds) override;

	virtual void TakeDamage(float DamageAmount) override;
	
	
};
