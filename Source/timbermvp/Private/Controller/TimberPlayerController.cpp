// Property of Paracosm Industries. Dont use my shit.


#include "Controller/TimberPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Character/TimberPlayableCharacter.h"

void ATimberPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
	
	if (Subsystem)
	{
		Subsystem->AddMappingContext(TimberInputMappingContext, 100);
	}
}

void ATimberPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	//Binding Move Function
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::Move);
}

void ATimberPlayerController::Move(const FInputActionValue& Value)
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Red, "Move Button Pressed",true);
	}

	APawn* ControlledPawn = GetPawn();

	check(ControlledPawn);

	const float ValueMagnitude = Value.GetMagnitude();

	const FVector Direction = ControlledPawn->GetActorForwardVector();

	ControlledPawn->AddMovementInput(Direction, 1.0f);
	
}


