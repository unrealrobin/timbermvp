// Property of Paracosm Industries. Dont use my shit.


#include "Controller/TimberPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Character/TimberPlayableCharacter.h"
#include "GameFramework/SpringArmComponent.h"

void ATimberPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
	
	if (Subsystem)
	{
		Subsystem->AddMappingContext(TimberInputMappingContext, 100);
	}
	
	TimberCharacter = Cast<ATimberPlayableCharacter>(GetPawn());
	TimberCharacterSpringArmComponent = TimberCharacter->GetSpringArmComponent();
}

void ATimberPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	//Binding Move Function
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::Move);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ATimberPlayerController::MoveComplete);
	EnhancedInputComponent->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::LookUp);
	EnhancedInputComponent->BindAction(LookRightAction, ETriggerEvent::Triggered, this, 
	&ATimberPlayerController::LookRight);
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

void ATimberPlayerController::LookUp(const FInputActionValue& Value)
{
	float ClampedPitch = FMath::Clamp(Value.Get<float>(), -0.5f, 0.5f);
	//Pitch is up and down
	TimberCharacter->AddControllerPitchInput(ClampedPitch);
}

void ATimberPlayerController::LookRight(const FInputActionValue& Value)
{
	float ClampedYaw = FMath::Clamp(Value.Get<float>(), -0.5f, 0.5f);
	//Yaw is left and right
	TimberCharacter->AddControllerYawInput(ClampedYaw);
}



