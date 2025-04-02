// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/Traps/TrapBase.h"
#include "Components/ActorComponent.h"
#include "BuildSystemManagerComponent.generated.h"


enum class EBuildingComponentOrientation : uint8;
enum class EBuildingComponentTrapDirection : uint8;
class ATeleportConstruct;
class ARampBase;
class ATrapBase;
class ABuildableBase;
class ATimberBuildingComponentBase;

USTRUCT(BlueprintType)
struct FBuildablePlacementData
{
	GENERATED_BODY()

	FBuildablePlacementData()
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

	// Called when a Buildable is Spawned, Makes the Game Mode Redraw Possible Paths from Doors to Seeda
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRedrawPathTraceHandle);
	FRedrawPathTraceHandle RedrawPathTraceHandle;

protected:
	virtual void BeginPlay() override;

	/* Buildable Utils */
	//Should match the Width of the most common Building Component. Used for Snapping.
	int GridSize = 100.f;

	//Class to be spawned with the SpawnActor function - Triggered by Selection in Building Component Icon Event Graph
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Buildable")
	TSubclassOf<ABuildableBase> ActiveBuildableComponentClass;

	//A Reference to the Proxy as a buildable - This should always be the reference to the Proxy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Buildable")
	ABuildableBase* BuildableProxyInstance = nullptr;

	/*UPROPERTY(EditAnywhere, Category="Buildable")
	ATimberBuildingComponentBase* ActiveBuildingComponentProxy = nullptr;*/

	/*UPROPERTY(EditAnywhere, Category="Buildable")
	ATrapBase* ActiveTrapComponentProxy = nullptr;*/

	/*UPROPERTY(EditAnywhere, Category="Buildable")
	ARampBase* ActiveRampComponentProxy = nullptr;*/
	
	/*UPROPERTY(EditAnywhere, Category="Buildable")
	ATeleportConstruct* ActiveTeleportConstructProxy = nullptr;*/

	/*
	 *This is a new approach where we group the Building Components by their Snapping Conditions instead of Play Type.
	 *The reason for this is that Traps and Constructs may share the exact same snapping conditions and some
	 *traps may have more complex and unique snapping conditions +
	 */ 
	/*UPROPERTY(EditAnywhere, Category="Buildable")
	ABuildableBase* CenterSnapFloorOnlyBuildingComponentProxy = nullptr;*/

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
	void VerticalToHorizontalSnapCondition(FHitResult HitActor, FHitResult HitQuadrant);
	UFUNCTION()
	void HorizontalToVerticalSnapCondition(FHitResult HitActor, FHitResult HitQuadrant);
	UFUNCTION()
	void MoveBuildingComponentProxyToSnapLocation(FVector ProxySnapLocation, FVector SnapLocation);
	UFUNCTION()
	void RotateProxyToSnapRotation(FRotator HitActorRotation, ABuildableBase* BuildingComponent);
	
	/*Static Mesh Utilities*/
	UPROPERTY(VisibleAnywhere, Category="Building Component")
	TArray<UStaticMeshComponent*> StaticMeshs;
	void GetStaticMeshComponents(AActor* BuildingComponentActor);
	UFUNCTION()
	void MakeMaterialHoloColor(AActor* BuildingComponentActor, UMaterial* HoloMaterialColor);
	void AddToBuildableAttachments(ABuildableBase* AttachingBuildable);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Building Component")
	UMaterial* RedHoloMaterial = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Building Component")
	UMaterial* YellowHoloMaterial = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Building Component")
	UMaterial* BlueHoloMaterial = nullptr;

	/*Collision Util*/
	void DisableBuildableProxyCollisions(ABuildableBase* BuildingComponent);

	/*SFX*/
	void PlayBuildablePlacementSound();
	void PlayBuildDeniedSound();

public:
	/* Spawning */
	/*UFUNCTION()
	void SpawnBuildingComponentProxy(FVector SpawnVector, FRotator SpawnRotator);*/
	/*UFUNCTION()
	void SpawnTrapComponentProxy(FVector_NetQuantize Location, FRotator SpawnRotator);*/
	UFUNCTION()
	void SpawnFinalBuildable();
	void SpawnFinalBuildingComponent(FActorSpawnParameters SpawnParameters);
	void SpawnFinalCenterSnapBuildable(FActorSpawnParameters SpawnParameters);
	void SpawnFinalFloorCenterSnapTopOnlyBuildable(FActorSpawnParameters SpawnParameters);
	void SpawnFinalRampBuildable(FActorSpawnParameters SpawnParameters);
	void SpawnFinalFloorEdgeSnapTopOnlyBuildable(FActorSpawnParameters SpawnParameters);

	void HandleIsTeleporter(ATeleportConstruct* TeleportConstruct);
	/*Placement*/

	//New Build Placement Functions
	UFUNCTION()
	void HandleProxyPlacement(TArray<FHitResult> HitResults, TSubclassOf<ABuildableBase> BuildableProxyClass);
	UFUNCTION()
	AActor* SpawnProxy(TSubclassOf<ABuildableBase> ActiveBuildableClass, FVector SpawnLocation, FRotator SpawnRotation);
	UFUNCTION()
	FHitResult FirstHitBuildingComponentHitResult(TArray<FHitResult> HitResults);
	UFUNCTION()
	void HandleBuildingComponentPlacement(FHitResult FirstHitBuildingComponentHitResult);
	UFUNCTION()
	void HandleCenterSnapPlacement(FHitResult FirstHitBuildingComponentHitResult);
	UFUNCTION()
	void HandleFloorCenterSnapTopOnlyPlacement(FHitResult FirstHitBuildingComponentHitResult);
	UFUNCTION()
	void HandleRampPlacement(FHitResult FirstHitBuildingComponentHitResult);
	UFUNCTION()
	void HandleFloorEdgeSnapTopOnlyPlacement(FHitResult FirstHitBuildingComponentHitResult);
	
	UFUNCTION()
	void HandleBuildingComponentSnapping(FHitResult HitResult);
	void MoveBuildable(
		FVector_NetQuantize Location, ABuildableBase* BuildingComponent, const FRotator& Rotation
			= FRotator::ZeroRotator);
	void RotateBuildingComponent();
	
	/*Used for Trap Placement*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trap Placement")
	bool bCanTrapsSnapToEnvironment = false;
	
	UFUNCTION()
	void HandleTrapMaterialChange(bool bCanTrapBeFinalized);
	
	FVector BuildingComponentImpactPoint;
	
	void RegisterTrapComponent(ATrapBase* TrapComponent);
	
	UFUNCTION()
	FORCEINLINE void ClearStoredStaticMeshes() { StaticMeshs.Empty(); };
	
	/*Clean up*/
	void ResetBuildableComponents(TSubclassOf<ABuildableBase> ActiveBuildableClass);
	void RemoveBuildingComponentProxies_All();
	
	/*Building Component Placement Variables*/
	FVector FinalSpawnLocation;
	FRotator FinalSpawnRotation;
	//FORCEINLINE void EmptyActiveBuildingComponent() { ActiveBuildingComponentProxy = nullptr; };

	/*Getters & Setters*/
	FORCEINLINE TSubclassOf<ABuildableBase> GetActiveBuildableClass() { return ActiveBuildableComponentClass; } ;
	UFUNCTION(BlueprintCallable, Category="Building Component")
	void SetActiveBuildingComponentClass(TSubclassOf<AActor> BuildingComponentClass);
	//Used to remember the previous Rotation of the Building Component after Leaving Build Mode.
	void SetSavedRotation(FRotator Rotation) { SavedRotation = Rotation; };
	/*UFUNCTION()
	FORCEINLINE ATrapBase* GetActiveRampComponent() const { return ActiveTrapComponentProxy; };*/
	//FORCEINLINE ATimberBuildingComponentBase* GetActiveBuildingComponentProxy() const { return ActiveBuildingComponentProxy; };
	//FORCEINLINE void SetActiveBuildingComponentToNull() { ActiveBuildingComponentProxy = nullptr; };
	/*FORCEINLINE void SetActiveTrapComponentToNull() { ActiveTrapComponentProxy = nullptr; };*/
	/*FORCEINLINE void SetActiveRampComponentToNull() { ActiveRampComponentProxy = nullptr; };
	FORCEINLINE void SetActiveRampComponent(ARampBase* RampComponent) { ActiveRampComponentProxy = RampComponent; };*/
	/*FORCEINLINE ABuildableBase* GetBuildableRef() { return BuildableProxyInstance; };*/
	/*FORCEINLINE void SetBuildingRef(ABuildableBase* BuildingComponent) { BuildableProxyInstance = BuildingComponent; };*/
	
	/*Component Snapping */
	UFUNCTION(BlueprintCallable, Category="Building")
	EBuildingComponentOrientation CheckClassBuildingComponentOrientation(AActor* ClassToBeChecked);


private:
	/*Buildable Placement Functions*/
	//Used for Traps and Constructs
	FBuildablePlacementData GetTrapSnapTransform(
		FVector ImpactPoint, ATimberBuildingComponentBase*
		BuildingComponent, ATrapBase* TrapComponentProxy);

	/*Teleport Utils*/
	TPair<ATeleportConstruct*, ATeleportConstruct*> TeleportTempPair;

	USceneComponent* GetClosestFaceSnapPoint(FHitResult HitResult);

	/*Tutorial*/
	UPROPERTY()
	float TutorialBuildsPlaced = 0;
};

