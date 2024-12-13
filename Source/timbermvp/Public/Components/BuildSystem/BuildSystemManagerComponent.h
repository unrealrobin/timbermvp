// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildSystemManagerComponent.generated.h"


class IBuildable;
enum class EBuildingComponentOrientation : uint8;
class ATimberBuildingComponentBase;

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

	//Class to be spawned with the SpawnActor function
	UPROPERTY(EditAnywhere, Category="Building Component")
	TSubclassOf<AActor> ActiveBuildingComponentClass;


	// Can be either a Trap or a Building Component
	//UPROPERTY(VisibleAnywhere, Category = "Building Component")
	
	

	// Name Change Recommended :  Proxy Building Component
	// Actor Reference to be stored after player selects a building component to use
	UPROPERTY(EditAnywhere, Category="Building Component")
	ATimberBuildingComponentBase* ActiveBuildingComponentProxy = nullptr;

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
	void RotateProxyToSnapRotation(FRotator HitActorRotation);
	UFUNCTION()
	void VerticalToHorizontalSnapCondition(FHitResult HitActor, FHitResult HitQuadrant);
	UFUNCTION()
	void HorizontalToVerticalSnapCondition(FHitResult HitActor, FHitResult HitQuadrant);
	UFUNCTION()
	void MoveProxyToSnapLocation(FVector ProxySnapLocation, FVector SnapLocation);


	/* @params - GhostOpacity */
	void MakeBuildingComponentProxy(AActor* BuildingComponentProxy);
	//How transparent to make the Proxy Material Color
	float GhostOpacity = 0.5f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void HandleBuildingComponentSnapping(FHitResult HitQuadrant, FHitResult HitActor);

	UFUNCTION()
	void SpawnBuildingComponentProxy(FVector SpawnVector, FRotator SpawnRotator);
	void MoveBuildingComponent(FVector_NetQuantize Location);
	void RotateBuildingComponent();
	
	UFUNCTION()
	void SpawnFinalBuildingComponent(const FVector& Location, const FRotator& Rotation);
	FVector FinalSpawnLocation;
	FRotator FinalSpawnRotation;
	
	UFUNCTION()
	ATimberBuildingComponentBase* GetActiveBuildingComponent();
	FORCEINLINE void EmptyActiveBuildingComponent() {ActiveBuildingComponentProxy = nullptr;};

	/*Getters & Setters*/
	UFUNCTION(BlueprintCallable, Category="Building Component")
	FORCEINLINE void SetActiveBuildingComponentClass(TSubclassOf<AActor> BuildingComponentClass) {ActiveBuildingComponentClass = BuildingComponentClass;};
	FORCEINLINE TSubclassOf<AActor> GetActiveBuildingComponentClass() {return ActiveBuildingComponentClass;} ;
	void SetSavedRotation(FRotator Rotation) {SavedRotation = Rotation;};

	/*Component Snapping */
	UFUNCTION(BlueprintCallable, Category="Building")
	EBuildingComponentOrientation CheckClassBuildingComponentOrientation(AActor* ClassToBeChecked);
		
};
