// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TimberCharacterBase.generated.h"

/* Forward Declarations*/

class UMovementComponent;
class USkeletalMeshComponent;

UCLASS()
class TIMBERMVP_API ATimberCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ATimberCharacterBase();

protected:
	virtual void BeginPlay() override;


public:
	
	void AddOverlayMaterialToCharacter(UMaterialInterface* MaterialToOverlay, float TimeBeforeRemoval);
	
	void RemoveOverlayMaterialFromCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="OverlayMaterial")
	UMaterialInterface* HitMarkerOverlayMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="OverlayMaterial")
	UMaterialInterface* PowerUpOverlayMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="OverlayMaterial")
	UMaterialInterface* ShieldOverlayMaterial;

	
};
