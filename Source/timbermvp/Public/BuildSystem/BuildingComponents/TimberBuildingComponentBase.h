// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/BuildableBase.h"
#include "BuildSystem/Traps/TrapBase.h"
#include "GameFramework/Actor.h"
#include "Styling/SlateBrush.h"
#include "TimberBuildingComponentBase.generated.h"

/*Forward Declarations*/
class UBoxComponent;

UENUM(BlueprintType)
enum class EBuildingComponentOrientation : uint8
{
	Vertical UMETA(DisplayName = "Vertical"),
	Horizontal UMETA(DisplayName = "Horizontal"),
	Default UMETA(DisplayName = "Default"),
};

UENUM(BlueprintType)
enum class EBuildingComponentType : uint8
{
	BasicWall UMETA(DisplayName = "BasicWall"),
	BasicFloorWall UMETA(DisplayName = "BasicFloor"),
	Default UMETA(DisplayName = "Default"),
};

UCLASS()
class TIMBERMVP_API ATimberBuildingComponentBase : public ABuildableBase
{
	GENERATED_BODY()
	
public:
	
	ATimberBuildingComponentBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Building Component Attributes")
	float ComponentDurability = 100.f;

	UFUNCTION()
	void BuildingComponentTakeDamage(float AmountOfDamage);
	UFUNCTION()
	void PlayDestroyedAnimation();

	void CreateSnapPoints();
	void CreateQuadrantComponents();

	/*States*/
	UPROPERTY(VisibleAnywhere, Category="Building Component Info" )
	EBuildingComponentOrientation BuildingOrientation = EBuildingComponentOrientation::Default;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Building Component Info")
	EBuildingComponentType BuildingComponentType = EBuildingComponentType::Default;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent* NavCollisionBox;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Snap Locations")
	USceneComponent* TopSnap;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Snap Locations")
	USceneComponent* BottomSnap;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Snap Locations")
	USceneComponent* RightSnap;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Snap Locations")
	USceneComponent* LeftSnap;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Snap Locations")
	USceneComponent* CenterSnap;

	/* Trap Snap Locations & Slots */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Snap Locations")
	USceneComponent* FrontTrapSnap;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Snap Locations")
	USceneComponent* BackTrapSnap;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Active Traps")
	ATrapBase* FrontTrap = nullptr;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Active Traps")
	ATrapBase* BackTrap = nullptr;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Quadrants")
	UBoxComponent* TopQuadrant;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Quadrants")
	UBoxComponent* BottomQuadrant;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Quadrants")
	UBoxComponent* RightQuadrant;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Quadrants")
	UBoxComponent* LeftQuadrant;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Quadrants")
	UBoxComponent* CenterQuadrant;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsUnlocked = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int MaterialCost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush BuildingComponentIconImage;

	UFUNCTION()
	void HandleOverlapNotifies(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
};
