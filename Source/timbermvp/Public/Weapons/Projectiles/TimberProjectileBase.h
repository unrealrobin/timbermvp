// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Character/TimberPlayableCharacter.h"
#include "GameFramework/Actor.h"
#include "TimberProjectileBase.generated.h"

class UCapsuleComponent;
class UProjectileMovementComponent;

UCLASS()
class TIMBERMVP_API ATimberProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimberProjectileBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ATimberPlayableCharacter* ProjectileOwner;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Projectile Combat Info")
	float ProjectileBaseDamage;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Weapon Projectiles")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
