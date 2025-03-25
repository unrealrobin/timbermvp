// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "TimberBuildingComponentBase.h"
#include "TimberVerticalBuildingComponent.generated.h"

UCLASS()
class TIMBERMVP_API ATimberVerticalBuildingComponent : public ATimberBuildingComponentBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATimberVerticalBuildingComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buildable Type")
	ESnapCondition SnapCondition = ESnapCondition::BuildingComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
