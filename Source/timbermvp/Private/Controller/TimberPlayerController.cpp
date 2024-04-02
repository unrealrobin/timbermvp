// Property of Paracosm Industries. Dont use my shit.


#include "Controller/TimberPlayerController.h"

#include "EnhancedInputSubsystems.h"

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
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ATimberPlayerController::MoveComplete);
}

void ATimberPlayerController::Move(const FInputActionValue& Value)
{

	InputActionValue = Value;

	APawn* ControlledPawn = GetPawn();

	if (ControlledPawn)
	{
		ForwardMoveDirection = ControlledPawn->GetActorForwardVector();
		RightMoveDirection = ControlledPawn->GetActorRightVector();
		
		ControlledPawn->AddMovementInput(ForwardMoveDirection, Value.Get<FVector2D>().X, false);
		ControlledPawn->AddMovementInput(RightMoveDirection, Value.Get<FVector2D>().Y, false);
	}
}

void ATimberPlayerController::MoveComplete(const FInputActionValue& Value)
{
	InputActionValue = FVector2d(0.f,0.f);
}



