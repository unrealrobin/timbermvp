// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Loot/EnemyLootDropBase.h"
#include "BuildableBase.generated.h"

USTRUCT(BlueprintType)
struct FBuildableCost
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Buildable Cost")
	int CostOfParts = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Buildable Cost")
	int CostOfMechanisms = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Buildable Cost")
	int CostOfUniques = 0;
};


UENUM(BlueprintType)
enum class EBuildableType : uint8
{
	Trap UMETA(DisplayName = "Trap"),
	BuildingComponent UMETA(DisplayName = "BuildingComponent"),
	Ramp UMETA(DisplayName = "Ramp"),
	Default UMETA(DisplayName = "Default"),
};

UCLASS()
class TIMBERMVP_API ABuildableBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuildableBase();
	EBuildableType BuildableType = EBuildableType::Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Buildable Cost")
	FBuildableCost BuildableCost;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	void SpawnLootInRange(int NumberOfParts = 0, int NumberOfMechanisms = 0, int NumberOfUniques = 0);

	/*Potential Loot on Destruction / Gaurunteed on Deletion */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Loot Drop Items")
	TSubclassOf<AEnemyLootDropBase> PartsClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Loot Drop Items")
	TSubclassOf<AEnemyLootDropBase> MechanismsClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Loot Drop Items")
	TSubclassOf<AEnemyLootDropBase> UniquesClass;

public:
	void HandleDeletionOfBuildable();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
