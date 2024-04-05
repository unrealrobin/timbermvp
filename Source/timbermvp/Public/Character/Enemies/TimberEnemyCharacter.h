// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Character/TimberCharacterBase.h"
#include "TimberEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TIMBERMVP_API ATimberEnemyCharacter : public ATimberCharacterBase
{
	GENERATED_BODY()

	ATimberEnemyCharacter();

public:
	virtual void BeginPlay() override;


protected:
	virtual void Tick(float DeltaSeconds) override;

	
	
	
};
