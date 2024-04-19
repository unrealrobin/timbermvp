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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TotalHealth = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float CurrentHealth = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float InitialSpeed = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MaxForwardSpeed = 800.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MaxBackwardSpeed = 500.f;
	
	
};
