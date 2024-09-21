// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimberBuildSystemManager.generated.h"

/* PREPARE TO BE DELETED. NOT NEEDED AND NOT REFERENCED.*/


class ATimberBuildingComponentBase;

UCLASS()
class TIMBERMVP_API ATimberBuildSystemManager : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ATimberBuildSystemManager();

protected:
	
	virtual void BeginPlay() override;

	int GridSize = 100.f;

	// This is the Building Component that Has not been placed in the world yet.
	// It is set in the BuildSystemManager BP. (In the future, user selection from UI will set this.)
	//The Template Type here needs to be actor for the Spawn "Actor" function to work correctly.
	UPROPERTY(EditAnywhere, Category="Building Component")
	TSubclassOf<AActor> ActiveBuildingComponentClass;
	UPROPERTY(EditAnywhere, Category="Building Component")
	ATimberBuildingComponentBase* ActiveBuildingComponent = nullptr;

	FVector SnapToGrid(FVector RaycastLocation);
	FRotator SnapToRotation(FRotator CharactersRotation);
	FRotator SavedRotation = FRotator::ZeroRotator;
	
	void MakeBuildingComponentProxy(ATimberBuildingComponentBase* BuildingComponent);
	float GhostOpacity = 0.5f;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SpawnBuildingComponent(FVector SpawnVector, FRotator SpawnRotator);
	void MoveBuildingComponent(FVector_NetQuantize Location);
	void RotateBuildingComponent();
	UFUNCTION()
	void SpawnFinalBuildingComponent(const FVector& Location, const FRotator& Rotation);
	FVector FinalSpawnLocation;
	FRotator FinalSpawnRotation;
	UFUNCTION()
	ATimberBuildingComponentBase* GetActiveBuildingComponent();
	FORCEINLINE void EmptyActiveBuildingComponent() {ActiveBuildingComponent = nullptr;};

	/*Getters & Setters*/
	UFUNCTION(BlueprintCallable, Category="Building Component")
	FORCEINLINE void SetActiveBuildingComponentClass(TSubclassOf<AActor> BuildingComponentClass) {ActiveBuildingComponentClass = BuildingComponentClass;};
	FORCEINLINE TSubclassOf<AActor> GetActiveBuildingComponentClass() {return ActiveBuildingComponentClass;} ;
};
