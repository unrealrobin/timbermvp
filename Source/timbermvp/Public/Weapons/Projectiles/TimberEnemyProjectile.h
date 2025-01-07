// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "TimberProjectileBase.h"
#include "TimberEnemyProjectile.generated.h"

UCLASS()
class TIMBERMVP_API ATimberEnemyProjectile : public ATimberProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATimberEnemyProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HandleOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
