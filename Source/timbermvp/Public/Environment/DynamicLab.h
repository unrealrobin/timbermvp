// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicLab.generated.h"

class ABuildableBase;

UCLASS()
class TIMBERMVP_API ADynamicLab : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADynamicLab();

private:
	UFUNCTION()
	void HandleFloorLayout();
	void EmptyChildComponentArray();

	void BuildLab();
	
	void HandleFloorLayoutChildComponent();

	void GenerateChildComponent(TSubclassOf<AActor> BuildableActor, FVector Location, FRotator Rotation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lab Details")
	TArray<UChildActorComponent*> ChildComponents;

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Lab Details")
	int NumberOfFloors = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lab Meshes")
	TSubclassOf<AActor> EnvironmentWallsClass = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lab Meshes")
	TSubclassOf<AActor> EnvironmentFloors = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lab Meshes")
	TSubclassOf<AActor> LabDoors = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lab Meshes")
	TSubclassOf<AActor> GarageDoor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lab Meshes")
	TSubclassOf<AActor> CeilingLights = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lab Meshes")
	float FloorSizeX = 395.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lab Meshes")
	float FloorSizeY = 395.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lab Details")
	int LabFloors = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lab Details")
	int LabLength = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lab Details")
	int LabWidth = 1;


	
};
