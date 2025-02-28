﻿// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "BossBase.h"
//#include "Character/Enemies/TimberEnemyCharacter.h"
#include "BossBruiser.generated.h"

class UBoxComponent;

UCLASS()
class TIMBERMVP_API ABossBruiser : public ABossBase
{
	GENERATED_BODY()

public:
	void SetupCapsuleComponents();
	// Sets default values for this character's properties
	ABossBruiser();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BruiserInfo")
	UCapsuleComponent* HeadCapsuleComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BruiserInfo")
	UBoxComponent* BodyCollisionComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BruiserInfo")
	UCapsuleComponent* RightArmCapsuleComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BruiserInfo")
	UCapsuleComponent* LeftArmCapsuleComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BruiserInfo")
	UCapsuleComponent* WhirlwindRightCollisionSphere;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BruiserInfo")
	UCapsuleComponent* WhirlwindLeftCollisionSphere;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BruiserInfo")
	UCapsuleComponent* OverHeadSmashCapsuleComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BruiserInfo")
	USceneComponent* OverHeadSmashCapsuleSpawnLocation;

	UFUNCTION(BlueprintCallable)
	void EnableCollisionToDamagePlayerOnly(UCapsuleComponent* WWCapsuleComponent);
	UFUNCTION(BlueprintCallable)
	void DisableCollisionToDamagePlayerOnly(UCapsuleComponent* WWCapsuleComponent);
	UFUNCTION()
	void HandleWhirlwindOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void HandleBHandSlapOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void HandleOverHeadSmashOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent*
		OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	void SpawnOverHeadCapsule();
	UFUNCTION(BlueprintCallable)
	void DestroyOverHeadCapsule();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bruiser Info")
	float HeadShotDamageModifier = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bruiser Info")
	float WhirlwindTickDamage = 4.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bruiser Info")
	float BHandSlapDamage = 8.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bruiser Info")
	float OverHeadSmashDamage = 25.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bruiser Info")
	float OverHeadSmashCapsuleHeight = 250.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bruiser Info")
	float OverHeadSmashCapsuleRadius = 250.f;
};
