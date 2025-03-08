// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/BuildableBase.h"
#include "GameFramework/Actor.h"
#include "TrapBase.generated.h"

class ATimberEnemyCharacter;
class ATimberBuildingComponentBase;
class UBoxComponent;

UENUM(BlueprintType)
enum class ETrapType : uint8
{
	SpikeTrap UMETA(DisplayName = "SpikeTrap"),
	FrostTrap UMETA(DisplayName = "FrostTrap"),
	Default UMETA(DisplayName = "Default"),
};

UENUM(BlueprintType)
enum class EBuildingComponentTrapDirection : uint8
{
	Front UMETA(DisplayName = "Front Snap"),
	Back UMETA(DisplayName = "Back Snap"),
	Default UMETA(DisplayName = "Default")
};

UCLASS()
class TIMBERMVP_API ATrapBase : public ABuildableBase
{
	GENERATED_BODY()

public:
	ATrapBase();

	/*Ref to Attached BC*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trap Components")
	ATimberBuildingComponentBase* ParentBuildingComponent;

protected:
	virtual void BeginPlay() override;

	void DisableAllStaticMeshCollisions(UStaticMeshComponent* SomeMesh);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Trap Components")
	bool CanTrapBeFinalized = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Hit Enemies")
	TArray<AActor*> InsideHitBoxArray;

public:
	virtual void Tick(float DeltaTime) override;
	void FreeUpTrapSlotOnBuildingComponent();

	//Used for Independent Deletion. When player deletes a specific Trap.
	virtual void HandleDeletionOfBuildable() override;
	
	//Used for deletion when the Parent Building Component is deleted.
	void HandleDeletionByBuildingComponent();

	/*Delegates*/
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTrapFinalizationChange, bool, CanTrapBeFinalized);
	FOnTrapFinalizationChange OnTrapFinalizationChange;
	
	/* Components */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trap Components")
	UStaticMeshComponent* TrapBaseStaticMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trap Components")
	UBoxComponent* HitBoxComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush BuildingComponentIconImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trap Components")
	USceneComponent* TrapCenterSnapLocation;
	
	/* Placement Utilities */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building Component")
	ATimberBuildingComponentBase* TrapHoveredBuildingComponent = nullptr;
	
	/* ENUMS */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trap Components")
	ETrapType TrapType = ETrapType::Default;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trap Components")
	EBuildingComponentTrapDirection BuildingComponentTrapDirection = EBuildingComponentTrapDirection::Default;

	/* Getters / Setters */
	FORCEINLINE bool GetCanTrapBeFinalized() const { return CanTrapBeFinalized; };
	
	void SetCanTrapBeFinalized(bool bCanTrapBeFinalized);

	/*Hit Area Utilities*/
	/* Adds Enemies to the Inside Hit Box Array*/
	UFUNCTION()
	virtual void HitBoxBeginOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	/* Removes Enemies from the Inside Hit Box Array*/
	UFUNCTION()
	virtual void HitBoxEndOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void AddEnemyToInsideHitBoxArray(AActor* Enemy);
	
	void RemoveEnemyFromInsideHitBoxArray(AActor* Enemy);
};
