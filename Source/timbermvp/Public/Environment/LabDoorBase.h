// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "LabDoorBase.generated.h"

struct FTimeline;
class ATimberEnemyCharacter;
class UBoxComponent;

UCLASS()
class TIMBERMVP_API ALabDoorBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALabDoorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Closed Position for Doors.
	FVector LabDoorLeftClosePos;
	FVector LabDoorRightClosePos;
	FVector LabDoorLeftOpenPos;
	FVector LabDoorRightOpenPos;

	//Timers for Testing Open and Closing of Lab Doors
	FTimerHandle LabDoorOpenTimerHandle;
	FTimerHandle LabDoorCloseTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lab Door")
	TArray<ATimberEnemyCharacter*> EnemiesInLabDoorActivator;

	UPROPERTY()
	UTimelineComponent* DoorOpenTimeline;
	UPROPERTY(BlueprintReadWrite, Category="Animation")
	UCurveFloat* FloatCurve;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lab Door Meshes")
	UBoxComponent* LabDoorActivatorComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lab Door Meshes")
	UStaticMeshComponent* LabDoorFrame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lab Door Meshes")
	UStaticMeshComponent* LabDoorLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lab Door Meshes")
	UStaticMeshComponent* LabDoorRight;

	UFUNCTION(BlueprintCallable)
	void OpenLabDoor(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void CloseLabDoor(float DeltaTime);

	UFUNCTION()
	void HandleOpenLabDoor(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleCloseLabDoor(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
	void SetupLabDoorComponents();
	// Debug Functions for Opening and closing of lab doors in the console in editor.
	UFUNCTION()
	void SetLabDoorToBeOpen();
	UFUNCTION()
	void SetLabDoorToBeClosed();
	UFUNCTION()
	void HandleTestLabDoorOpen();
	UFUNCTION()
	void InitializeLabDoorProperties();

	UPROPERTY(VisibleAnywhere)
	bool ShouldLabDoorBeOpen = false;

	UPROPERTY(VisibleAnywhere)
	bool IsLabDoorOpen = false;
};
