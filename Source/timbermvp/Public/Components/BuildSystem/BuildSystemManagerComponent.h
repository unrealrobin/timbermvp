// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildSystemManagerComponent.generated.h"


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

	// Name Change Recommended :  Proxy Building Component
	// Actor Reference to be stored after player selects a building component to use
	UPROPERTY(EditAnywhere, Category="Building Component")
	ATimberBuildingComponentBase* ActiveBuildingComponent = nullptr;
	
	FVector SnapToGrid(FVector RaycastLocation);
	FRotator SnapToRotation(FRotator CharactersRotation);
	FRotator SavedRotation = FRotator::ZeroRotator;

	/* @params - GhostOpacity */
	void MakeBuildingComponentProxy(ATimberBuildingComponentBase* BuildingComponent);
	//How transparent to make the Proxy Material Color
	float GhostOpacity = 0.5f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void HandleBuildingComponentSnapping(FHitResult HitResult);

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
	FORCEINLINE void EmptyActiveBuildingComponent() {ActiveBuildingComponent = nullptr;};

	/*Getters & Setters*/
	UFUNCTION(BlueprintCallable, Category="Building Component")
	FORCEINLINE void SetActiveBuildingComponentClass(TSubclassOf<AActor> BuildingComponentClass) {ActiveBuildingComponentClass = BuildingComponentClass;};
	FORCEINLINE TSubclassOf<AActor> GetActiveBuildingComponentClass() {return ActiveBuildingComponentClass;} ;
	void SetSavedRotation(FRotator Rotation) {SavedRotation = Rotation;};
		
};
