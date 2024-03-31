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
	int TotalHealth = 100;
	int CurrentHealth = 100;

	UFUNCTION()
	void DecreaseHealth(int DecreaseAmount);

	

};
