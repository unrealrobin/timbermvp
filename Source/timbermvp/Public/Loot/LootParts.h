// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "EnemyLootDropBase.h"
#include "LootParts.generated.h"

UCLASS()
class TIMBERMVP_API ALootParts : public AEnemyLootDropBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALootParts();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleLootItemOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
