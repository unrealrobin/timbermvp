// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Animation/AnimInstance.h"
#include "TimberAnimInstance.generated.h"


class ATimberPlayerController;
class ATimberCharacterBase;
/**
 * 
 */
UCLASS()
class TIMBERMVP_API UTimberAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	FInputActionValue InputActionValue;

private:

	ATimberCharacterBase* TimberCharacterBase;
	ATimberPlayerController* TimberPlayerController;
	
};
