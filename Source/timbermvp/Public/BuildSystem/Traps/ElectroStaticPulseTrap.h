﻿// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "PlusTrapBase.h"
#include "ElectroStaticPulseTrap.generated.h"

/*
 *Shoots A Web of Electro Static Energy that Damages and Leaves a Damage over Time effect on Enemy.
 *Slow Cooldown - 10 sec?
 */

UCLASS()
class TIMBERMVP_API AElectroStaticPulseTrap : public APlusTrapBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AElectroStaticPulseTrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MovePulseHitBox(float DeltaTime);

	UFUNCTION()
	void FireElectroPulse();

	UFUNCTION()
	void HandlePulseBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float InitialHitDamage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* PulseHitBox;
	
	FTimerHandle FireElectroPulseTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooldown")
	float FireElectroPulseCooldown = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMesh* PulseMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PulseMeshComponent = nullptr;

	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void CreatePulseHitBox();
};
