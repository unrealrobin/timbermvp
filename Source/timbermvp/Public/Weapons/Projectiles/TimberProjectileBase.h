// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	UCapsuleComponent* CapsuleComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Weapon Projectiles")
	UProjectileMovementComponent* ProjectileMovementComponent;

};
