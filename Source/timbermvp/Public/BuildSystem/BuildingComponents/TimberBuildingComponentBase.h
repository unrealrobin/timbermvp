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
	Environment UMETA(DisplayName = "Environment"),
	Default UMETA(DisplayName = "Default"),
};

UCLASS()
class TIMBERMVP_API ATimberBuildingComponentBase : public ABuildableBase
{
	GENERATED_BODY()

public:
	
	ATimberBuildingComponentBase();

	UFUNCTION()
	void HandleHitBuildingComponent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void SetupProxyCollisionHandling();

	// Used to check overlap with other building component.
	UFUNCTION()
	void HandleOverlappedBuildingComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void HandleEndOverlappedBuildingComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Building Component Attributes")
	float ComponentDurability = 100.f;

	UFUNCTION()
	void BuildingComponentTakeDamage(float AmountOfDamage, AActor* DamagingActor);
	void CreateSnapPoints();
	void CreateQuadrantComponents();

	/*States*/
	UPROPERTY(VisibleAnywhere, Category="Building Component Info")
	EBuildingComponentOrientation BuildingOrientation = EBuildingComponentOrientation::Default;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Building Component Info")
	EBuildingComponentType BuildingComponentType = EBuildingComponentType::Default;

	/*Proxy*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Building Component")
	bool bIsProxy = false;

	/*Attached Traps/Constructs*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attached Building Components")
	TArray<ABuildableBase*> AttachedBuildingComponents;

	UFUNCTION()
	void DestroyAllAttachments();

	virtual void Destroyed() override;

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush BuildingComponentIconImage;
	
};
