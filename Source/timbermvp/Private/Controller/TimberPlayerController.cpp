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
	EnhancedInputComponent->BindAction(LookRightAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::LookRight);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::CharacterJump);
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::Interact);
	EnhancedInputComponent->BindAction(EquipWeaponOneAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::EquipWeaponOne);
	EnhancedInputComponent->BindAction(EquipWeaponTwoAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::EquipWeaponTwo);
	EnhancedInputComponent->BindAction(EquipWeaponThreeAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::EquipWeaponThree);
	EnhancedInputComponent->BindAction(StandardAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::StandardAttack);
	
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
		//Controller Rotation and Forward Vector
		const FRotator ControllerRotation = GetControlRotation();
		const FVector ControllerForwardVector = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::X);

		//Current Directions of the Character
		CharacterForwardMoveDirection = ControlledPawn->GetActorForwardVector();
		CharacterRightMoveDirection = ControlledPawn->GetActorRightVector();

		// Rotates the Character to the direction of the Camera Smoothly
		const FRotator TargetForwardDirection = FRotator(0.f, ControllerRotation.Yaw, 0.f);
		FRotator NewRotation = FMath::RInterpTo(ControlledPawn->GetActorRotation(), TargetForwardDirection, GetWorld()->GetDeltaSeconds(), 5.f);
		ControlledPawn->SetActorRotation(NewRotation);

		//Adding Movement Input to the Character
		ControlledPawn->AddMovementInput(ControllerForwardVector, Value.Get<FVector2D>().X, false);
		ControlledPawn->AddMovementInput(CharacterRightMoveDirection, Value.Get<FVector2D>().Y, false);
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
	
	SetControlRotation(UpdatedRotation);
}

void ATimberPlayerController::LookRight(const FInputActionValue& Value)
{
	FRotator UpdatedRotation = TimberCharacter->GetControlRotation();
	UpdatedRotation.Yaw = UpdatedRotation.Yaw + Value.Get<float>();
	SetControlRotation(UpdatedRotation);
}

void ATimberPlayerController::CanCharacterJump()
{
	bool bIsMovingOnGround = TimberCharacterMovementComponent->IsMovingOnGround();
	bool bIsIdle = TimberCharacterMovementComponent->Velocity.Size() == 0.f;
	bool IsFalling = TimberCharacterMovementComponent->IsFalling();

	if(IsFalling) CanJump = false;
	
	if(bIsMovingOnGround || bIsIdle )
	{
		CanJump = true;
	}
	else
	{
		CanJump = false;
	}
}

void ATimberPlayerController::JumpComplete()
{
	SwitchToWalking = true;
	CanJump = false;
}

void ATimberPlayerController::CharacterJump(const FInputActionValue& Value)
{
	CanCharacterJump();
	SwitchToWalking = false;
	if(CanJump)
	{
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
		}
	}
	
	
}

void ATimberPlayerController::EquipWeaponOne(const FInputActionValue& Value)
{
	if(TimberCharacter)
	{
		//Setting WeaponState on Character
		TimberCharacter->SetCurrentWeaponState(EWeaponState::AxeEquipped);
		
		// Spawning and Attaching the Weapon to the Socket of Right Hand on Leeroy
		const FActorSpawnParameters SpawnParams;

		//Socket Rotation and Location
		const FVector HandSocketLocation = TimberCharacter->GetMesh()->GetSocketLocation("AxeSocket");
		const FRotator HandSocketRotation = TimberCharacter->GetMesh()->GetSocketRotation("AxeSocket");

		FTransform SocketWorldTransform = TimberCharacter->GetMesh()->GetSocketTransform("AxeSocket", ERelativeTransformSpace::RTS_World);
		FVector SocketWorldLocation = SocketWorldTransform.GetLocation();
		FRotator SocketWorldRotation = SocketWorldTransform.Rotator();
		
		

		//Spawn the Actor
		ATimberWeaponBase* SpawnedActor = GetWorld()->SpawnActor<ATimberWeaponBase>(TimberCharacter->WeaponOne, 
		SocketWorldLocation, SocketWorldRotation, SpawnParams);

		//Attach Actor to the Socket Location
		SpawnedActor->AttachToComponent(TimberCharacter->GetMesh(), 
		FAttachmentTransformRules::SnapToTargetIncludingScale, "AxeSocket");

	

		//Set the Newly Spawned Weapon to the WeaponOneInstance on Leeroy
		TimberCharacter->WeaponOneInstance = SpawnedActor;

		//Set Leeroy on the Owner of the Weapon so we can Reference the Owner from the Weapon.
		SpawnedActor->SetOwner(TimberCharacter);
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

void ATimberPlayerController::StandardAttack(const FInputActionValue& Value)
{
	if(TimberCharacter && TimberCharacter->GetCurrentWeaponState() != EWeaponState::Unequipped)
	{
		//TODO:: Play Animation for Axe Standard Melee Attack
		switch (TimberCharacter->GetCurrentWeaponState())
		{
		case EWeaponState::AxeEquipped:
			{
				TimberCharacter->WeaponOneInstance->HandlePlayAttackMontage();
			}
			break;
		case EWeaponState::ChainsawEquipped:
			{
				GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, "Attacking with Chainsaw");
			}
			break;
		case EWeaponState::PistolEquipped:
			{
				GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, "Attacking with Pistol");
			}
			break;
		case EWeaponState::Unequipped:
			{
				GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, "No Weapon Equipped");
			}
			break;
		}
	}
}





