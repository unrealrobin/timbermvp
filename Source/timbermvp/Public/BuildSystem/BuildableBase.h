// Property of Paracosm Industries.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
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
enum class ESnapCondition : uint8
{
	/* Used for Walls and Floors*/
	BuildingComponent UMETA(DisplayName = "BuildingComponent"),
	/* Used for Traps that can be placed on Front or Back of Walls and Floors*/
	CenterSnap UMETA(DisplayName = "CenterSnap"),
	/* Used for items like the Power-plate that need to be walked over. - Facing +Z */
	FloorCenterSnapTopOnly UMETA(DisplayName = "FloorCenterSnapTopOnly"),
	/* Used for Items that may only hand from ceiling - Facing -Z*/
	FloorCenterSnapBottomOnly UMETA(DisplayName = "FloorCenterSnapBottomOnly"),
	/*Used for Teleporters*/
	FloorEdgeSnapTopOnly UMETA(DisplayName = "FloorCenterSnapBottomOnly"),
	/*Used for Ramps and Stairs Only*/
	Ramp UMETA(DisplayName = "Ramp"),
	/*Default Setting needs to be Overwritten on the Individual Buildables*/
	Default UMETA(DisplayName = "Default"),
};


UENUM(BlueprintType)
enum class EBuildableType : uint8
{
	BuildingComponent UMETA(DisplayName = "BuildingComponent"),
	Ramp UMETA(DisplayName = "Ramp"),
	Trap UMETA(DisplayName = "Trap"),
	Construct UMETA(DisplayName = "Construct"),
	Environment UMETA(DisplayName = "Environment"),
	Default UMETA(DisplayName = "Default"),
};

UCLASS()
class TIMBERMVP_API ABuildableBase : public AActor, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*Unique Tag for Tracking/Saving/Loading */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Buildable")
	FGuid GUID;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Buildable")
	USceneComponent* RootSceneComponent;
	
	void SpawnLootInRange(int NumberOfParts = 0, int NumberOfMechanisms = 0, int NumberOfUniques = 0);

	/*Potential Loot on Destruction / Guaranteed on Deletion */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Loot Drop Items")
	TSubclassOf<AEnemyLootDropBase> PartsClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Loot Drop Items")
	TSubclassOf<AEnemyLootDropBase> MechanismsClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Loot Drop Items")
	TSubclassOf<AEnemyLootDropBase> UniquesClass;
	
public:
	// Sets default values for this actor's properties
	ABuildableBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Building Component Attributes")
	float ComponentDurability = 100.f;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	bool bCanBuildableBeFinalized = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buildable Type")
	ESnapCondition SnapCondition = ESnapCondition::Default;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buildable Type")
	EBuildableType BuildableType = EBuildableType::Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Buildable Cost")
	FBuildableCost BuildableCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Tags")
	FGameplayTagContainer GameplayTagContainer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Buildable")
	ABuildableBase* ParentBuildable;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	
	UFUNCTION()
	virtual void HandleDeletionOfBuildable();

	UFUNCTION()
	FGuid GetGUID();
	
	UFUNCTION()
	void SetGUID(FGuid NewGUID);
	
};
