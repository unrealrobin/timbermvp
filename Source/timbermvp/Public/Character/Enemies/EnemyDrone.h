// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "TimberEnemyCharacter.h"
#include "EnemyDrone.generated.h"

class USplineComponent;
class AEnemyDroneSplinePath;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline Paths")
	bool bShouldMoveAlongSpline = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline Paths")
	bool bIsApproachSpline = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline Paths")
	TArray<AActor*> SplinePathActors; // Array to hold all spline paths in the level
	
	void SelectRandomSplinePath();
	
	void GetAllDroneSplinePathActors();

	void MoveAlongSplinePath(float DeltaTime);

	void FlyToSplineStart(float DeltaTime);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline Paths")
	float CurrentDistanceAlongSplinePath = 0.0f; // Distance along the spline path

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Path")
	AEnemyDroneSplinePath* SplinePathRef; // Reference to the spline path this drone will follow
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Path")
	USplineComponent* SplineComponent; // Reference to the spline component of the selected spline path

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Path")
	float DroneApproachSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Path")
	float DroneFlightSpeed = 300.0f;

};
