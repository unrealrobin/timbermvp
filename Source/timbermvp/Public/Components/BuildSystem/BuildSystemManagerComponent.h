// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/Traps/TrapBase.h"
#include "Components/ActorComponent.h"
#include "BuildSystemManagerComponent.generated.h"


enum class EBuildingComponentOrientation : uint8;
enum class EBuildingComponentTrapDirection : uint8;
class ARampBase;
class ATrapBase;
class ABuildableBase;
class ATimberBuildingComponentBase;

USTRUCT(BlueprintType)
struct FTrapSnapData
{
	GENERATED_BODY()

	FTrapSnapData()
		: TrapLocation(FVector::ZeroVector), TrapRotation(FRotator::ZeroRotator)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trap Snap Data")
	FVector TrapLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Trup Snap Data")
	FRotator TrapRotation;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TIMBERMVP_API UBuildSystemManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBuildSystemManagerComponent();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRedrawPathTraceHandle);
	FRedrawPathTraceHandle RedrawPathTraceHandle;
	

protected:
	virtual void BeginPlay() override;

	/* Buildable Utils */
	//Should match the Width of the most common Building Component.
	int GridSize = 100.f;

	//Class to be spawned with the SpawnActor function - Triggered by Selection in Building Component Icon Event Graph
	UPROPERTY(EditAnywhere, Category="Building Component")
	TSubclassOf<ABuildableBase> ActiveBuildableComponentClass;

	UPROPERTY(EditAnywhere, Category="Building Component")
	ABuildableBase* BuildableRef = nullptr;

	UPROPERTY(EditAnywhere, Category="Building Component")
	ATimberBuildingComponentBase* ActiveBuildingComponentProxy = nullptr;

	UPROPERTY(EditAnywhere, Category="Trap Component")
	ATrapBase* ActiveTrapComponentProxy = nullptr;

	UPROPERTY(EditAnywhere, Category="Ramp Component")
	ARampBase* ActiveRampComponentProxy = nullptr;

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


	
	/*Static Mesh Utilities*/
	UPROPERTY(VisibleAnywhere, Category="Building Component")
	TArray<UStaticMeshComponent*> StaticMeshs;
	void GetStaticMeshComponents(AActor* BuildingComponentActor);
	UFUNCTION()
	void MakeMaterialHoloColor(AActor* BuildingComponentActor, UMaterial* HoloMaterialColor);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Building Component")
	UMaterial* RedHoloMaterial = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Building Component")
	UMaterial* YellowHoloMaterial = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Building Component")
	UMaterial* BlueHoloMaterial = nullptr;

public:
	/* Spawning */
	UFUNCTION()
	void SpawnBuildingComponentProxy(FVector SpawnVector, FRotator SpawnRotator);
	UFUNCTION()
	void SpawnFinalBuildable();
	void SpawnFinalRampComponent(FActorSpawnParameters SpawnParameters);
	void SpawnFinalTrap(FActorSpawnParameters SpawnParameters);
	void SpawnFinalBuildingComponent(FActorSpawnParameters SpawnParameters);

	/*Placement*/
	UFUNCTION()
	void HandleRampPlacement(TArray<FHitResult> HitResults);
	UFUNCTION()
	void HandleTrapPlacement(TArray<FHitResult> HitResults);
	UFUNCTION()
	void HandleBuildingComponentPlacement(TArray<FHitResult> HitResults);
	UFUNCTION()
	void HandleTrapMaterialChange(bool bCanTrapBeFinalized);
	FVector BuildingComponentImpactPoint;

	/*Registering Buildable*/
	void RegisterTrapComponent(ATrapBase* TrapComponent);

	UFUNCTION()
	FORCEINLINE void ClearStoredStaticMeshes() { StaticMeshs.Empty(); };

	UFUNCTION()
	void HandleBuildingComponentSnapping(FHitResult HitQuadrant, FHitResult HitActor);
	void ResetBuildableComponents(TSubclassOf<ABuildableBase> ActiveBuildableClass);
	void RemoveBuildingComponentProxies_All();

	

	UFUNCTION()
	void SpawnTrapComponentProxy(FVector_NetQuantize Location, FRotator SpawnRotator);
	void MoveBuildingComponent(
		FVector_NetQuantize Location, ABuildableBase* BuildingComponent, const FRotator& Rotation
			= FRotator::ZeroRotator);
	void RotateBuildingComponent();

	FVector FinalSpawnLocation;
	FRotator FinalSpawnRotation;

	FORCEINLINE void EmptyActiveBuildingComponent() { ActiveBuildingComponentProxy = nullptr; };

	/*Getters & Setters*/
	FORCEINLINE TSubclassOf<ABuildableBase> GetActiveBuildableClass() { return ActiveBuildableComponentClass; } ;
	UFUNCTION()
	FORCEINLINE ATimberBuildingComponentBase* GetActiveBuildingComponent() const
	{
		return ActiveBuildingComponentProxy;
	};
	UFUNCTION()
	FORCEINLINE ATrapBase* GetActiveRampComponent() const { return ActiveTrapComponentProxy; };
	UFUNCTION()
	FORCEINLINE ATrapBase* GetActiveTrapComponent() const { return ActiveTrapComponentProxy; };
	UFUNCTION(BlueprintCallable, Category="Building Component")
	void SetActiveBuildingComponentClass(TSubclassOf<AActor> BuildingComponentClass);
	
	FORCEINLINE void SetActiveBuildingComponentToNull() { ActiveBuildingComponentProxy = nullptr; };
	FORCEINLINE void SetActiveTrapComponentToNull() { ActiveTrapComponentProxy = nullptr; };
	FORCEINLINE void SetActiveRampComponentToNull() { ActiveRampComponentProxy = nullptr; };
	FORCEINLINE void SetActiveRampComponent(ARampBase* RampComponent) { ActiveRampComponentProxy = RampComponent; };
	FORCEINLINE void SetBuildableRefToNull() { BuildableRef = nullptr; };
	FORCEINLINE ABuildableBase* GetBuildableRef() { return BuildableRef; };
	FORCEINLINE void SetBuildingRef(ABuildableBase* BuildingComponent) { BuildableRef = BuildingComponent; };

	void SetSavedRotation(FRotator Rotation) { SavedRotation = Rotation; };

	/*Buildable Placement Functions*/
	FTrapSnapData GetTrapSnapTransform(
		FVector ImpactPoint, ATimberBuildingComponentBase*
		BuildingComponent, ATrapBase* TrapComponent);


	/*Component Snapping */
	UFUNCTION(BlueprintCallable, Category="Building")
	EBuildingComponentOrientation CheckClassBuildingComponentOrientation(AActor* ClassToBeChecked);
};
