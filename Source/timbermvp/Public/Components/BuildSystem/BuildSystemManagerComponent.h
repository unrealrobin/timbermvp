// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/Traps/TrapBase.h"
#include "Components/ActorComponent.h"
#include "BuildSystemManagerComponent.generated.h"


enum class EBuildingComponentTrapDirection : uint8;
class ATrapBase;
class ABuildableBase;
class IBuildable;
enum class EBuildingComponentOrientation : uint8;
class ATimberBuildingComponentBase;

USTRUCT(BlueprintType)
struct FTrapSnapData
{
	GENERATED_BODY()

public:
	FTrapSnapData()
		: TrapLocation(FVector::ZeroVector), TrapRotation(FRotator::ZeroRotator)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trap Snap Data")
	FVector TrapLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Trup Snap Data")
	FRotator TrapRotation;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TIMBERMVP_API UBuildSystemManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuildSystemManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Should match the Width of the most common Building Component.
	int GridSize = 100.f;

	//Class to be spawned with the SpawnActor function - Triggered by Selection in Building Component Icon Event Graph
	UPROPERTY(EditAnywhere, Category="Building Component")
	TSubclassOf<ABuildableBase> ActiveBuildableComponentClass;
	
	// Actor Reference to be stored after player selects a building component to use
	UPROPERTY(EditAnywhere, Category="Building Component")
	ATimberBuildingComponentBase* ActiveBuildingComponentProxy = nullptr;

	UPROPERTY(EditAnywhere, Category="Trap Component")
	ATrapBase* ActiveTrapComponentProxy = nullptr;

	/*Grid Snap*/
	FVector SnapToGrid(FVector RaycastLocation);
	FRotator SnapToRotation(FRotator CharactersRotation);
	FRotator SavedRotation = FRotator::ZeroRotator;

	/*Component Snapping*/
	int SnappingCondition(EBuildingComponentOrientation Orientation1, EBuildingComponentOrientation Orientation2);
	int QuadrantCondition(FString QuadrantName);
	UFUNCTION()
	void SameOrientationSnapCondition(FHitResult HitActor, FHitResult HitQuadrant);
	UFUNCTION()
	void RotateProxyToSnapRotation(FRotator HitActorRotation, ABuildableBase* BuildingComponent);
	UFUNCTION()
	void VerticalToHorizontalSnapCondition(FHitResult HitActor, FHitResult HitQuadrant);
	UFUNCTION()
	void HorizontalToVerticalSnapCondition(FHitResult HitActor, FHitResult HitQuadrant);
	UFUNCTION()
	void MoveProxyToSnapLocation(FVector ProxySnapLocation, FVector SnapLocation);


	/* @params - GhostOpacity */
	void MakeBuildingComponentProxy(AActor* BuildingComponentProxy);
	bool SpawnFinalTrap(FActorSpawnParameters SpawnParameters);
	void SpawnFinalBuildingComponent(FActorSpawnParameters SpawnParameters);
	//How transparent to make the Proxy Material Color
	float GhostOpacity = 0.5f;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void HandleBuildingComponentSnapping(FHitResult HitQuadrant, FHitResult HitActor);
	void ResetBuildableComponents(TSubclassOf<ABuildableBase> ActiveBuildableClass);

	UFUNCTION()
	void SpawnBuildingComponentProxy(FVector SpawnVector, FRotator SpawnRotator);
	UFUNCTION()
	void SpawnTrapComponentProxy(FVector_NetQuantize Location, FRotator SpawnRotator);
	void MoveBuildingComponent(FVector_NetQuantize Location, ABuildableBase* BuildingComponent, const FRotator& Rotation 
	= FRotator::ZeroRotator);
	void RotateBuildingComponent();
	
	UFUNCTION()
	void SpawnFinalBuildingComponent();
	
	FVector FinalSpawnLocation;
	FRotator FinalSpawnRotation;
	
	FORCEINLINE void EmptyActiveBuildingComponent() {ActiveBuildingComponentProxy = nullptr;};
	/*Getters & Setters*/
	FORCEINLINE TSubclassOf<ABuildableBase> GetActiveBuildableClass() {return ActiveBuildableComponentClass;} ;
	
	UFUNCTION()
	FORCEINLINE ATimberBuildingComponentBase* GetActiveBuildingComponent() const {return ActiveBuildingComponentProxy;};

	UFUNCTION()
	FORCEINLINE ATrapBase* GetActiveTrapComponent() const {return ActiveTrapComponentProxy;};
	
	UFUNCTION(BlueprintCallable, Category="Building Component")
	FORCEINLINE void SetActiveBuildingComponentClass(TSubclassOf<AActor> BuildingComponentClass) {ActiveBuildableComponentClass = BuildingComponentClass;};
	FORCEINLINE void SetActiveBuildingComponentToNull() {ActiveBuildingComponentProxy = nullptr;};
	FORCEINLINE void SetActiveTrapComponentToNull() {ActiveTrapComponentProxy = nullptr;};
	void SetSavedRotation(FRotator Rotation) {SavedRotation = Rotation;};

	/*Buildable Placement Functions*/

	FTrapSnapData GetTrapSnapTransform(
		FVector ImpactPoint, ATimberBuildingComponentBase* 
		BuildingComponent, ATrapBase* TrapComponent);
	

	/*Component Snapping */
	UFUNCTION(BlueprintCallable, Category="Building")
	EBuildingComponentOrientation CheckClassBuildingComponentOrientation(AActor* ClassToBeChecked);
		
};
