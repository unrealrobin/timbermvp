// Property of Paracosm Industries. Dont use my shit.


#include "Controller/TimberPlayerController.h"

void ATimberPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	//Binding Move Function
	EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Ongoing, this, &ATimberPlayerController::Move);
}

void ATimberPlayerController::Move(const FInputActionValue& Value)
{
}


