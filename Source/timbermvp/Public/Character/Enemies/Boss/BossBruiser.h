// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemies/TimberEnemyCharacter.h"
#include "BossBruiser.generated.h"

UCLASS()
class TIMBERMVP_API ABossBruiser : public ATimberEnemyCharacter
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
	UCapsuleComponent* RightArmCapsuleComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BruiserInfo")
	UCapsuleComponent* LeftArmCapsuleComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BruiserInfo")
	UCapsuleComponent* WhirlwindRightCollisionSphere;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BruiserInfo")
	UCapsuleComponent* WhirlwindLeftCollisionSphere;

	UFUNCTION(BlueprintCallable)
	void EnableCollisionToDamagePlayerOnly(UCapsuleComponent* WWCapsuleComponent);
	UFUNCTION(BlueprintCallable)
	void DisableCollisionToDamagePlayerOnly(UCapsuleComponent* WWCapsuleComponent);
	UFUNCTION()
	void HandleWhirlwindOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void HandleBHandSlapOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bruiser Info")
	float HeadShotDamageModifier = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bruiser Info")
	float WhirlwindTickDamage = 4.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bruiser Info")
	float BHandSlapDamage = 8.0f;
};
