// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicLab.generated.h"

class ABuildableBase;

UENUM(BlueprintType) // Allows usage in Blueprints
enum class ELabDimension : uint8
{
	S UMETA(DisplayName = "Small"), 
	M UMETA(DisplayName = "Medium"),
	L UMETA(DisplayName = "Large"),
	XL UMETA(DisplayName = "Extra Large"),
};

UCLASS()
class TIMBERMVP_API ADynamicLab : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADynamicLab();

private:
	void EmptyChildComponentArray();

	void BuildLab();

	void SetDimensionsFromState(ELabDimension LabDimensionEnum);

	void UpdateLabDimensions(int x, int y, int z);
	
	void HandleFloorLayout();
	
	void HandleWallLayout();

	void HandleCeilingLightLayout(FVector Location);

	void HandleBossSpawnArea();
	
	void GenerateChildComponent(TSubclassOf<AActor> BuildableActor, FVector Location, FRotator Rotation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lab Details")
	TArray<UChildActorComponent*> ChildComponents;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere, Category = "Lab Details")
	int LabLevels = 1;

	UPROPERTY(VisibleAnywhere, Category = "Lab Details")
	int LabLength = 1;

	UPROPERTY(VisibleAnywhere, Category = "Lab Details")
	int LabWidth = 1;
	
	UPROPERTY(VisibleAnywhere, Category = "Lab Meshes")
	float FloorSizeX = 395.0f;

	UPROPERTY(VisibleAnywhere, Category = "Lab Meshes")
	float FloorSizeY = 395.0f;

	UPROPERTY(VisibleAnywhere, Category = "Lab Meshes")
	float WallSizeZ = 395.0f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lab Details")
	ELabDimension LabDimension = ELabDimension::L;

	UPROPERTY(VisibleAnywhere, Category = "Lab Details")
	int NumberOfFloors = 0;

	UPROPERTY(VisibleAnywhere, Category = "Lab Details")
	int NumberOfWalls = 0;

	UPROPERTY(VisibleAnywhere, Category = "Lab Details")
	int NumberOfCeilingTiles = 0;

	UPROPERTY(VisibleAnywhere, Category = "Lab Details")
	int NumberOfLabDoor = 0;

	UPROPERTY(VisibleAnywhere, Category = "Lab Details")
	int NumberOfCeilingLights = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lab Details")
	float CeilingLightDropOffset = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lab Meshes")
	TSubclassOf<AActor> EnvironmentWalls = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lab Meshes")
	TSubclassOf<AActor> EnvironmentFloors = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lab Meshes")
	TSubclassOf<AActor> LabDoors = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lab Meshes")
	TSubclassOf<AActor> GarageDoor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lab Meshes")
	TSubclassOf<AActor> CeilingLights = nullptr;

	
};
