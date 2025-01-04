// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/BuildableBase.h"
#include "RampBase.generated.h"

class UBoxComponent;

UCLASS()
class TIMBERMVP_API ARampBase : public ABuildableBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARampBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ramp Component")
	UBoxComponent* RootComponentBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ramp Component")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ramp Component")
	USceneComponent* VerticalCenterSnap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ramp Component")
	USceneComponent* HorizontalCenterSnap;
	
	
};
