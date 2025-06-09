// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "NavigationPath.h"
#include "Components/ActorComponent.h"
#include "NavigationHelperComponent.generated.h"


class ABuildableBase;
class ATimberCharacterBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TIMBERMVP_API UNavigationHelperComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UNavigationHelperComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Navigation Data")
	AActor* OwningActor = nullptr;

	UFUNCTION()
	UNavigationPath* GetOriginalNavPath(FVector Start, FVector End);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Navigation Logic")
	TArray<FVector> GetCorridorPathPoints(FVector Start, FVector End);

	bool CheckIfPathShouldUpdate(ABuildableBase* BuildableActor);
};
