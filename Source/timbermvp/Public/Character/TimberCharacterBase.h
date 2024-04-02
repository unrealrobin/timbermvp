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
	
	/*UPROPERTY(EditAnywhere, Category="Character Component")
	UMovementComponent* CharacterMovementComponent;*/

public:
	UPROPERTY(BlueprintReadOnly)
	float TotalHealth = 100.0f;
	UPROPERTY(BlueprintReadOnly)
	float CurrentHealth = 100.0f;
	UPROPERTY(BlueprintReadOnly)
	float InitialSpeed = 0.f;
	UPROPERTY(BlueprintReadOnly)
	float MaxForwardSpeed = 600.f;
	UPROPERTY(BlueprintReadOnly)
	float MaxBackwardSpeed = 600.f;

	UFUNCTION()
	void DecreaseHealth(int DecreaseAmount);

	

};
