// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "TimberEnemyCharacter.h"
#include "EnemyDrone.generated.h"

UCLASS()
class TIMBERMVP_API AEnemyDrone : public ATimberEnemyCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyDrone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
