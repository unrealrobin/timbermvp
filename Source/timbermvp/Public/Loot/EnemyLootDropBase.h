// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemyLootDropBase.generated.h"

class UCapsuleComponent;

UCLASS()
class TIMBERMVP_API AEnemyLootDropBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyLootDropBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetCollisionProperties();

public:

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Component Details")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Component Details")
	UCapsuleComponent* CollisionCapsule;

	

};
