// Property of Paracosm Industries. Dont use my shit.


#include "Controller/TimberPlayerController.h"
#include "Interfaces/Interactable.h"
#include "EnhancedInputSubsystems.h"
#include "BuildSystem/TimberBuildingComponentBase.h"
#include "BuildSystem/TimberBuildSystemManager.h"
#include "Character/TimberPlayableCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/TimberWeaponBase.h"

void ATimberPlayerController::BeginPlay()
{
	Super::BeginPlay();

	Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
	
	if (Subsystem)
	{
		Subsystem->AddMappingContext(StandardInputMappingContext, 1);
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
	EnhancedInputComponent->BindAction(ToggleBuildModeAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::ToggleBuildMode);
	EnhancedInputComponent->BindAction(RotateBuildingComponentAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::RotateBuildingComponent);
	EnhancedInputComponent->BindAction(PlaceBuildingComponentAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::PlaceBuildingComponent);
	
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
	PitchAngle = UpdatedRotation.Pitch;
}

void ATimberPlayerController::LookRight(const FInputActionValue& Value)
{
	FRotator UpdatedRotation = TimberCharacter->GetControlRotation();
	UpdatedRotation.Yaw = UpdatedRotation.Yaw + Value.Get<float>();
	SetControlRotation(UpdatedRotation);
	YawAngle = UpdatedRotation.Yaw;
}

void ATimberPlayerController::CanCharacterJump()
{
	//Checks conditions to ensure character is not already in a jump or falling off something already.
	bool bIsMovingOnGround = TimberCharacterMovementComponent->IsMovingOnGround();
	bool bIsIdle = TimberCharacterMovementComponent->Velocity.Size() == 0.1f;
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
	TimberCharacter->IsNowJumping = false; //Is the Bool we use to Switch between Animations Tracks for Blending Purposes.
	SwitchToWalking = true;
	CanJump = false;
}

void ATimberPlayerController::CharacterJump(const FInputActionValue& Value)
{
	CanCharacterJump(); // sets the CanJump Variable
	SwitchToWalking = false;
	if(CanJump)
	{
		TimberCharacter->IsNowJumping = true; //Is the Bool we use to Switch between Animations Tracks for Blending Purposes.
		if(TimberCharacter->IsNowJumping)
		{
			TimberCharacter->Jump();
		}
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
		
		//TODO:: Play Equip Axe Animation
		UnEquipWeapon();
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

		//Set the Newly Spawned Weapon to the WeaponOneInstance and CurrentlyEquippedWeapon on Leeroy
		TimberCharacter->WeaponOneInstance = SpawnedActor;
		TimberCharacter->SetCurrentlyEquippedWeapon(SpawnedActor);
		//Set Leeroy on the Owner of the Weapon so we can Reference the Owner from the Weapon.
		SpawnedActor->SetOwner(TimberCharacter);
	}
}

void ATimberPlayerController::EquipWeaponTwo(const FInputActionValue& Value)
{
	TimberCharacter->SetCurrentWeaponState(EWeaponState::ChainsawEquipped);

	check(TimberCharacter);

	UnEquipWeapon();
	
	TimberCharacter->SetCurrentWeaponState(EWeaponState::ChainsawEquipped);

	// Spawning and Attaching the Weapon to the Socket of Right Hand on Leeroy
	const FActorSpawnParameters SpawnParams;

	//Socket Rotation and Location
	const FVector HandSocketLocation = TimberCharacter->GetMesh()->GetSocketLocation("ChainSawSocket");
	const FRotator HandSocketRotation = TimberCharacter->GetMesh()->GetSocketRotation("ChainSawSocket");

	FTransform SocketWorldTransform = TimberCharacter->GetMesh()->GetSocketTransform("ChainSawSocket", ERelativeTransformSpace::RTS_World);
	FVector SocketWorldLocation = SocketWorldTransform.GetLocation();
	FRotator SocketWorldRotation = SocketWorldTransform.Rotator();

	//Spawn the Actor
	ATimberWeaponBase* SpawnedActor = GetWorld()->SpawnActor<ATimberWeaponBase>(TimberCharacter->WeaponTwo, 
	SocketWorldLocation, SocketWorldRotation, SpawnParams);

	//Attach Actor to the Socket Location
	SpawnedActor->AttachToComponent(TimberCharacter->GetMesh(), 
	FAttachmentTransformRules::SnapToTargetIncludingScale, "ChainSawSocket");

	//Set the Newly Spawned Weapon to the WeaponOneInstance and CurrentlyEquippedWeapon on Leeroy
	TimberCharacter->WeaponTwoInstance = SpawnedActor;
	TimberCharacter->SetCurrentlyEquippedWeapon(SpawnedActor);
	//Set Leeroy on the Owner of the Weapon so we can Reference the Owner from the Weapon.
	SpawnedActor->SetOwner(TimberCharacter);
	
}

void ATimberPlayerController::EquipWeaponThree(const FInputActionValue& Value)
{
	check(TimberCharacter);

	UnEquipWeapon();
	
	TimberCharacter->SetCurrentWeaponState(EWeaponState::RangedEquipped);

	// Spawning and Attaching the Weapon to the Socket of Right Hand on Leeroy
	const FActorSpawnParameters SpawnParams;

	//Socket Rotation and Location
	const FVector HandSocketLocation = TimberCharacter->GetMesh()->GetSocketLocation("RangedSocket");
	const FRotator HandSocketRotation = TimberCharacter->GetMesh()->GetSocketRotation("RangedSocket");

	FTransform SocketWorldTransform = TimberCharacter->GetMesh()->GetSocketTransform("RangedSocket", ERelativeTransformSpace::RTS_World);
	FVector SocketWorldLocation = SocketWorldTransform.GetLocation();
	FRotator SocketWorldRotation = SocketWorldTransform.Rotator();

	//Spawn the Actor
	ATimberWeaponBase* SpawnedActor = GetWorld()->SpawnActor<ATimberWeaponBase>(TimberCharacter->WeaponThree, 
	SocketWorldLocation, SocketWorldRotation, SpawnParams);

	//Attach Actor to the Socket Location
	SpawnedActor->AttachToComponent(TimberCharacter->GetMesh(), 
	FAttachmentTransformRules::SnapToTargetIncludingScale, "RangedSocket");

	//Set the Newly Spawned Weapon to the WeaponOneInstance and CurrentlyEquippedWeapon on Leeroy
	TimberCharacter->WeaponThreeInstance = SpawnedActor;
	TimberCharacter->SetCurrentlyEquippedWeapon(SpawnedActor);
	//Set Leeroy on the Owner of the Weapon so we can Reference the Owner from the Weapon.
	SpawnedActor->SetOwner(TimberCharacter);
	
}

void ATimberPlayerController::StandardAttack(const FInputActionValue& Value)
{
	if(TimberCharacter && TimberCharacter->GetCurrentWeaponState() != EWeaponState::Unequipped && CanAttackAgain)
	{
		switch (TimberCharacter->GetCurrentWeaponState())
		{
		case EWeaponState::AxeEquipped:
			{
				TimberCharacter->WeaponOneInstance->HandlePlayAttackMontage();
				CanAttackAgain = false;
			}
			break;
		case EWeaponState::ChainsawEquipped:
			{
				GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, "Attacking with Chainsaw");
			}
			break;
		case EWeaponState::RangedEquipped:
			{
				TimberCharacter->WeaponThreeInstance->FireRangedWeapon();
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

void ATimberPlayerController::ToggleBuildMode(const FInputActionValue& Value)
{
	
	TimberCharacter->CharacterState == ECharacterState::Building ? TimberCharacter->CharacterState = ECharacterState::Standard : 
	TimberCharacter->CharacterState = ECharacterState::Building;

	if(TimberCharacter->CharacterState == ECharacterState::Standard)
	{
		//WHen leaving building Mode, we need to empty the ActiveBuildingComponent. Why tho? Maybe Unnecessary.
		ATimberBuildSystemManager* BuildSystemManager = TimberCharacter->BuildSystemManagerInstance;
		if(BuildSystemManager)
		{
			//If player exits build mode with an active building component that isn't placed, destroy it.
			BuildSystemManager->GetActiveBuildingComponent()->Destroy();
			BuildSystemManager->EmptyActiveBuildingComponent();
		}

		if(Subsystem)
		{
			Subsystem->RemoveMappingContext(BuildModeInputMappingContext);
		}
	}else if (TimberCharacter->CharacterState == ECharacterState::Building)
	{
		if(Subsystem)
		{
			Subsystem->AddMappingContext(BuildModeInputMappingContext, 2);
		}
		UnEquipWeapon();
		//Setting WeaponState on Character
		TimberCharacter->SetCurrentWeaponState(EWeaponState::Unequipped);
		
	}
}

void ATimberPlayerController::RotateBuildingComponent(const FInputActionValue& Value)
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(3,5.0f, FColor::Green, "Q Key Pressed");
	}
	if(TimberCharacter->CharacterState == ECharacterState::Building && TimberBuildSystemManager)
	{
		TimberBuildSystemManager->RotateBuildingComponent();
		
	}
	
}

void ATimberPlayerController::PlaceBuildingComponent(const FInputActionValue& Value)
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(4, 5.0f, FColor::Green, "LMB Key Pressed in Build Mode");
	}

	//TODO:: Place Building Component with Correct Material.

	TimberBuildSystemManager->SpawnFinalBuildingComponent(
		TimberBuildSystemManager->FinalSpawnLocation, TimberBuildSystemManager->FinalSpawnRotation);

	//Spawn a new actor with the Correct Material at that location.
	//TODO:: Handle multiple building components overlapping each other. Do not allow placement.
}

void ATimberPlayerController::UnEquipWeapon() const
{
	//TODO:: Play Unequip Animation
	if(TimberCharacter->GetCurrentlyEquippedWeapon())
	{
		//Removing the Currently EquippedWeapon
		TimberCharacter->GetCurrentlyEquippedWeapon()->Destroy();
	}
}




