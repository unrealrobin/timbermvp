// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "TimberEnemyCharacter.h"
#include "Weapons/Projectiles/TimberProjectileBase.h"
#include "FloaterDrones.generated.h"


class ATimberEnemyProjectile;

UENUM(BlueprintType)
enum class EDroneState : uint8
{
	Damageable UMETA(DisplayName = "Damageable"),
	NotDamageable UMETA(DisplayName = "Not Damageable")
	
};

UCLASS()
class TIMBERMVP_API AFloaterDrones : public ATimberEnemyCharacter
{
	GENERATED_BODY()

public:
	AFloaterDrones();

	//Delegate Broadcast will be added to let Lola know that she is now damageable.
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDroneDestroyed, AFloaterDrones*, Drone);
	FDroneDestroyed DroneDestroyedHandle;

protected:

	virtual void TakeDamage(float DamageAmount, AActor* DamageInstigator) override;

	UFUNCTION()
	void HandleDroneDeath();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
	EDroneState DroneState = EDroneState::NotDamageable;

	UFUNCTION()
	void HandleBaseMaterialColorChange(EDroneState State);

	UFUNCTION()
	void SetActorUnDamageable();
	UFUNCTION()
	void SetActorDamageable();
	
public:

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HandleChangeDamageState(EDroneState NewState);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInterface* DamageableMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInterface* NotDamageableMaterial = nullptr;

	/* Projectile */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<ATimberProjectileBase> ProjectileClass;
	
	UFUNCTION()
	void FireProjectile();

	FTimerHandle FireProjectileTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float TimeBetweenShots = 2.f;
};
