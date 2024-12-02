// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LabDoorBase.generated.h"

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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

	// Debug Functions for Opening and closing of lab doors in the console in editor.
	UFUNCTION(Exec)
	void SetLabDoorToBeOpen();
	UFUNCTION(Exec)
	void SetLabDoorToBeClosed();

	UPROPERTY(VisibleAnywhere)
	bool ShouldLabDoorBeOpen = false;

	UPROPERTY(VisibleAnywhere)
	bool IsLabDoorOpen = false;
	
};
