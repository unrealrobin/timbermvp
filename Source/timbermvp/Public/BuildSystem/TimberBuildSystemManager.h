// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimberBuildSystemManager.generated.h"

class ATimberBuildingComponentBase;

UCLASS()
class TIMBERMVP_API ATimberBuildSystemManager : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ATimberBuildSystemManager();

protected:
	
	virtual void BeginPlay() override;

	float GridSize = 100.f;

	// This is the Building Component that Has not been placed in the world yet.
	// It is set in the BuildSystemManager BP. (In the future, user selection from UI will set this.)
	UPROPERTY(EditAnywhere, Category="Building Component")
	TSubclassOf<ATimberBuildingComponentBase> ActiveBuildingComponentClass;
	UPROPERTY(EditAnywhere, Category="Building Component")
	ATimberBuildingComponentBase* ActiveBuildingComponent;

public:	
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SpawnBuildingComponent(FVector SpawnVector, FRotator SpawnRotator);

};