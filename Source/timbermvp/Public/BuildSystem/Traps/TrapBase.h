// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/BuildableBase.h"
#include "GameFramework/Actor.h"
#include "TrapBase.generated.h"

class ATimberBuildingComponentBase;
class UBoxComponent;

UENUM(BlueprintType)
enum class ETrapType : uint8
{
	SpikeTrap UMETA(DisplayName = "SpikeTrap"),
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

protected:
	
	virtual void BeginPlay() override;

	void DisableAllStaticMeshCollisions(UStaticMeshComponent* SomeMesh);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Trap Components")
	bool CanTrapBeFinalized = false;

public:

	/*Delegates*/
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTrapFinalizationChange, bool, CanTrapBeFinalized);
	FOnTrapFinalizationChange OnTrapFinalizationChange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building Component")
	ATimberBuildingComponentBase* HoveredBuildingComponent = nullptr;
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trap Components")
	UStaticMeshComponent* TrapBaseStaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trap Components")
	USceneComponent* TrapCenterSnapLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trap Components")
	ETrapType TrapType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trap Components")
	UBoxComponent* DamageAreaBoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush BuildingComponentIconImage;
	
	EBuildingComponentTrapDirection BuildingComponentTrapDirection = EBuildingComponentTrapDirection::Default;

	/* Getters / Setters */

	FORCEINLINE bool GetCanTrapBeFinalized() const { return CanTrapBeFinalized; };
	void SetCanTrapBeFinalized(bool bCanTrapBeFinalized);
};
