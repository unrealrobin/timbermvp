// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "Styling/SlateBrush.h"
#include "TimberBuildingComponentBase.generated.h"

UCLASS()
class TIMBERMVP_API ATimberBuildingComponentBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimberBuildingComponentBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Building Component Attributes")
	float ComponentDurability = 100.f;

	UFUNCTION()
	void BuildingComponentTakeDamage(float AmountOfDamage);
	UFUNCTION()
	void PlayDestroyedAnimation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsUnlocked = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int MaterialCost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush BuildingComponentIconImage;

	UFUNCTION()
	void HandleOverlapNotifies(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
};
