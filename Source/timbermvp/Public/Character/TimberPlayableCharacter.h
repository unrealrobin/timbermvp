// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Character/TimberCharacterBase.h"
#include "TimberPlayableCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class TIMBERMVP_API ATimberPlayableCharacter : public ATimberCharacterBase
{
	GENERATED_BODY()

public:
	//Constructor
	ATimberPlayableCharacter();

	virtual void BeginPlay() override;

	/*Components*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	USpringArmComponent* CameraSpringArm;
	USpringArmComponent* GetSpringArmComponent() {return CameraSpringArm;}

	/*Attributes / Defaults*/
	bool IsRunning = true;
	float MaxRunSpeed = 1000.f;
};
