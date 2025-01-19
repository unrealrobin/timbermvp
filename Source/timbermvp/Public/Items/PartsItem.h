// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PartsItem.generated.h"

class UCapsuleComponent;

UCLASS()
class TIMBERMVP_API APartsItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APartsItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandlePartsOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

public:

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Component Details")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Component Details")
	UCapsuleComponent* CollisionCapsule;

	

};
