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
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, 
	&ATimberPlayerController::CharacterJump);
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
	const float CurrentPitch = TimberCharacter->GetControlRotation().Pitch;
	const float ClampedPitch = FMath::Clamp(CurrentPitch + Value.Get<float>(), ViewPitchMin, ViewPitchMax);
	
	FRotator UpdatedRotation = TimberCharacter->GetControlRotation();
	UpdatedRotation.Pitch = ClampedPitch;

	GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Red, FString::Printf(TEXT("Pitch: %f"), UpdatedRotation.Pitch));
	
	SetControlRotation(UpdatedRotation);
}

void ATimberPlayerController::LookRight(const FInputActionValue& Value)
{
	FRotator UpdatedRotation = TimberCharacter->GetControlRotation();
	UpdatedRotation.Yaw = UpdatedRotation.Yaw + Value.Get<float>();
	SetControlRotation(UpdatedRotation);
}

void ATimberPlayerController::CharacterJump(const FInputActionValue& Value)
{
	TimberCharacter->Jump();
}



