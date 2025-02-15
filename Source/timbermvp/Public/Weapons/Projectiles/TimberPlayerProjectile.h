// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "TimberProjectileBase.h"
#include "TimberPlayerProjectile.generated.h"

UCLASS()
class TIMBERMVP_API ATimberPlayerProjectile : public ATimberProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATimberPlayerProjectile();

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

	UFUNCTION()
	float CalculateOutputDamage(AActor* PlayerActor);
};
