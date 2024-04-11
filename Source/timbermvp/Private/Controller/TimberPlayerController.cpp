// Property of Paracosm Industries. Dont use my shit.


#include "Controller/TimberPlayerController.h"
#include "Interfaces/Interactable.h"
#include "EnhancedInputSubsystems.h"
#include "Character/TimberPlayableCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/TimberWeaponBase.h"


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
	TimberCharacterMovementComponent = TimberCharacter->GetCharacterMovement();
	TimberPlayerController = this;

	
	
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
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::Interact);
	EnhancedInputComponent->BindAction(EquipWeaponOneAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::EquipWeaponOne);
	EnhancedInputComponent->BindAction(EquipWeaponTwoAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::EquipWeaponTwo);
	EnhancedInputComponent->BindAction(EquipWeaponThreeAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::EquipWeaponThree);
	
}

void ATimberPlayerController::CantJump()
{
	CanJump = false;
}

void ATimberPlayerController::SetInteractableItem(IInteractable* Item)
{
	InteractableItem = Item;
}

void ATimberPlayerController::Move(const FInputActionValue& Value)
{
	MoveInputActionValue = Value;

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
	//This just resets the MoveInputActionValue to 0,0 after the last input is released. Otherwise the character will keep moving in the last direction.
	MoveInputActionValue = FVector2d(0.f,0.f);
	
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
	if(TimberCharacterMovementComponent->IsFalling())
	{
		CanJump = false;
	}else
	{
		CanJump = true;
		TimberCharacter->Jump();
	}
	
	
}

void ATimberPlayerController::Interact(const FInputActionValue& Value)
{
	if(Value.Get<bool>() && InteractableItem)
	{
		if(InteractableItem)
		{
			InteractableItem->Interact();
			if(GEngine)
			{
				GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, "E: Interacting with Item");
			}
		}
	}
	
	
}

void ATimberPlayerController::EquipWeaponOne(const FInputActionValue& Value)
{
	//TODO: Change Weapon State to EWeaponState::AxeEquipped
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, "Weapon 1 Equipped");
	}

	if(TimberCharacter && TimberCharacter->WeaponOne)
	{
		// Spawning and Attaching the Weapon to the Socket of Right Hand on Leeroy
		const FActorSpawnParameters SpawnParams;
		ATimberWeaponBase* SpawnedActor = GetWorld()->SpawnActor<ATimberWeaponBase>(TimberCharacter->WeaponOne, 
		FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f), SpawnParams);
		SpawnedActor->AttachToComponent(TimberCharacter->GetMesh(), 
		FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Hand_RSocket"));
	}
}

void ATimberPlayerController::EquipWeaponTwo(const FInputActionValue& Value)
{
	//TODO: Change Weapon State to EWeaponState::ChainsawEquipped
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, "Weapon 2 Equipped");
	}
}

void ATimberPlayerController::EquipWeaponThree(const FInputActionValue& Value)
{
	//TODO: Change Weapon State to EWeaponState::PistolEquipped
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, "Weapon 3 Equipped");
	}
}





