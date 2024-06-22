// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Styling/SlateBrush.h"
#include "TimberBuildingComponentBase.generated.h"

UCLASS()
class TIMBERMVP_API ATimberBuildingComponentBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimberBuildingComponentBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsUnlocked = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int MaterialCost = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FSlateBrush BuildingComponentIconImage;
};
