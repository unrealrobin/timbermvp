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
	void OpenLabDoor();	

	UFUNCTION(BlueprintCallable)
	void CloseLabDoor();
};
