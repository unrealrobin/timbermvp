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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Hit Enemies")
	TArray<ATimberEnemyCharacter*> InsideHitBoxArray;

public:

	virtual void Tick(float DeltaTime) override;
	
	/*Delegates*/
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTrapFinalizationChange, bool, CanTrapBeFinalized);
	FOnTrapFinalizationChange OnTrapFinalizationChange;

	/* Placement Utilities */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building Component")
	ATimberBuildingComponentBase* HoveredBuildingComponent = nullptr;
	
	/* Components */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trap Components")
	UStaticMeshComponent* TrapBaseStaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trap Components")
	USceneComponent* TrapCenterSnapLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trap Components")
	UBoxComponent* HitBoxComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush BuildingComponentIconImage;

	/* ENUMS */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trap Components")
	ETrapType TrapType;
	EBuildingComponentTrapDirection BuildingComponentTrapDirection = EBuildingComponentTrapDirection::Default;

	/* Getters / Setters */
	FORCEINLINE bool GetCanTrapBeFinalized() const { return CanTrapBeFinalized; };
	void SetCanTrapBeFinalized(bool bCanTrapBeFinalized);

	/*Hit Area Utilities*/
	virtual void HitBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void HitBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void AddEnemyToInsideHitBoxArray(ATimberEnemyCharacter* Enemy);
	void RemoveEnemyFromInsideHitBoxArray(ATimberEnemyCharacter* Enemy);
};
