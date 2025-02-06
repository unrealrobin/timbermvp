// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "TimberCharacterBase.h"
#include "Interfaces/Interactable.h"
#include "TimberSeeda.generated.h"

UCLASS()
class TIMBERMVP_API ATimberSeeda : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATimberSeeda();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSeedaDeathHandle, bool, bIsPlayerDead);
	FOnSeedaDeathHandle OnSeedaDeath;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSeedaInteractOverlap, bool, bShouldShowToolTip);
	FOnSeedaInteractOverlap OnSeedaInteractOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentHealth = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision Sphere")
	UCapsuleComponent* CollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision Sphere")
	UCapsuleComponent* InteractOverlapSphere;

	UFUNCTION()
	void TakeDamage_Seeda(float DamageAmount);

	UFUNCTION()
	void HandleCharacterBindingToSeeda();

	UFUNCTION()
	void RepairSeeda();

	/*Interface Functions*/

	UFUNCTION()
	void Interact() override;

	UFUNCTION()
	void AddInteractableToPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void RemoveInteractableFromPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
