// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "ConstructBase.h"
#include "TeleportConstruct.generated.h"

class UBoxComponent;

UCLASS()
class TIMBERMVP_API ATeleportConstruct : public AConstructBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATeleportConstruct();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* PillarLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* PillarRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* TeleportLandingLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* TeleportOverlapBox;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
