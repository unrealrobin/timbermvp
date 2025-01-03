// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildableBase.generated.h"

UENUM(BlueprintType)
enum class EBuildableType : uint8
{
	Trap UMETA(DisplayName = "Trap"),
	BuildingComponent UMETA(DisplayName = "BuildingComponent"),
	Ramp UMETA(DisplayName = "Ramp"),
	Default UMETA(DisplayName = "Default"),
};

UCLASS()
class TIMBERMVP_API ABuildableBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuildableBase();
	EBuildableType BuildableType = EBuildableType::Default;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
