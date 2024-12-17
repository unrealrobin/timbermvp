// Property of Paracosm Industries. Dont use my shit.


#include "Controller/TimberPlayerController.h"
#include "Interfaces/Interactable.h"
#include "EnhancedInputSubsystems.h"
#include "BuildSystem/BuildingComponents/TimberBuildingComponentBase.h"
#include "Weapons/TimberWeaponMeleeBase.h"
#include "Character/TimberPlayableCharacter.h"
#include "Components/BuildSystem/BuildSystemManagerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameModes/TimberGameModeBase.h"
#include "Kismet/GameplayStatics.h"

#include "Weapons/TimberWeaponBase.h"
#include "Weapons/TimberWeaponRangedBase.h"

void ATimberPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//TODO:: How can I make this better? Seems rigid. This can change mid game based on colliding objects, build locations, etc.
	// This is used for the Saving and Loading Process. We need a Location in space to respawn during load.
	APlayerStart* PlayerStartObject = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(),
	APlayerStart::StaticClass()));
	PlayerStartLocation = PlayerStartObject->GetActorLocation();
	
	Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
	
	TimberCharacter = Cast<ATimberPlayableCharacter>(GetPawn());
	TimberCharacterSpringArmComponent = TimberCharacter->GetSpringArmComponent();
	TimberCharacterMovementComponent = TimberCharacter->GetCharacterMovement();
	EnableStandardKeyboardInput();

	//Delegate Subscription
	TimberCharacter->HandlePlayerDeath_DelegateHandle.AddDynamic(this, &ATimberPlayerController::HandlePlayerDeath);

	ATimberGameModeBase* GameMode = Cast<ATimberGameModeBase>(GetWorld()->GetAuthGameMode());
	if(GameMode)
	{
		GameMode->EnableStandardInputMappingContext.BindUFunction(this, FName("EnableStandardKeyboardInput"));
	}
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
	EnhancedInputComponent->BindAction(HideBuildMenuAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::HideBuildMenu);
	EnhancedInputComponent->BindAction(DeleteBuildingComponentAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::DeleteBuildingComponent);
	
}

void ATimberPlayerController::MovePlayerToStartLocation()
{
	if(PlayerStartLocation != FVector(0.f,0.f,0.f))
	{
		TimberCharacter->SetActorLocation(PlayerStartLocation);
	}
}

void ATimberPlayerController::EnableCursor()
{
	bShowMouseCursor = true;
	USpringArmComponent* CharacterSpringArm = Cast<USpringArmComponent>(TimberCharacter->GetComponentByClass(USpringArmComponent::StaticClass()));
	CharacterSpringArm->bUsePawnControlRotation = false;
}

void ATimberPlayerController::DisableCursor()
{
	bShowMouseCursor = false;
	USpringArmComponent* CharacterSpringArm = Cast<USpringArmComponent>(TimberCharacter->GetComponentByClass(USpringArmComponent::StaticClass()));
	CharacterSpringArm->bUsePawnControlRotation = true;
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
		ExitBuildMode(ECharacterState::Standard);
		//Setting WeaponState on Character
		TimberCharacter->SetCurrentWeaponState(EWeaponState::AxeEquipped);
		WeaponState.Broadcast(EWeaponState::AxeEquipped);
		
		// Spawning and Attaching the Weapon to the Socket of Right Hand on Leeroy
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this; //Controller Owns the Object
		SpawnParams.Instigator = Cast<ATimberPlayableCharacter>(GetPawn()); // Instigator uses the object and instigates the actions with the Actor.

		//Socket Rotation and Location
		const FVector HandSocketLocation = TimberCharacter->GetMesh()->GetSocketLocation("AxeSocket");
		const FRotator HandSocketRotation = TimberCharacter->GetMesh()->GetSocketRotation("AxeSocket");

		FTransform SocketWorldTransform = TimberCharacter->GetMesh()->GetSocketTransform("AxeSocket", ERelativeTransformSpace::RTS_World);
		FVector SocketWorldLocation = SocketWorldTransform.GetLocation();
		FRotator SocketWorldRotation = SocketWorldTransform.Rotator();
		
		

		//Spawn the Weapon
		ATimberWeaponMeleeBase* SpawnedActor = GetWorld()->SpawnActor<ATimberWeaponMeleeBase>
		(TimberCharacter->WeaponOne, 
		SocketWorldLocation, SocketWorldRotation, SpawnParams);

		//Attach Actor to the Socket Location
		SpawnedActor->AttachToComponent(TimberCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "AxeSocket");

		//Set the Newly Spawned Weapon to the WeaponOneInstance and CurrentlyEquippedWeapon on Leeroy
		TimberCharacter->WeaponOneInstance = SpawnedActor;
		TimberCharacter->SetCurrentlyEquippedWeapon(SpawnedActor);
		
		
		
	}
}

void ATimberPlayerController::EquipWeaponTwo(const FInputActionValue& Value)
{
	TimberCharacter->SetCurrentWeaponState(EWeaponState::ChainsawEquipped);

	check(TimberCharacter);

	UnEquipWeapon();
	ExitBuildMode(ECharacterState::Standard);
	TimberCharacter->SetCurrentWeaponState(EWeaponState::ChainsawEquipped);
	WeaponState.Broadcast(EWeaponState::ChainsawEquipped);
	// Spawning and Attaching the Weapon to the Socket of Right Hand on Leeroy
	const FActorSpawnParameters SpawnParams;

	//Socket Rotation and Location
	const FVector HandSocketLocation = TimberCharacter->GetMesh()->GetSocketLocation("ChainSawSocket");
	const FRotator HandSocketRotation = TimberCharacter->GetMesh()->GetSocketRotation("ChainSawSocket");

	FTransform SocketWorldTransform = TimberCharacter->GetMesh()->GetSocketTransform("ChainSawSocket", ERelativeTransformSpace::RTS_World);
	FVector SocketWorldLocation = SocketWorldTransform.GetLocation();
	FRotator SocketWorldRotation = SocketWorldTransform.Rotator();

	//Spawn the Actor
	ATimberWeaponMeleeBase* SpawnedActor = GetWorld()->SpawnActor<ATimberWeaponMeleeBase>(TimberCharacter->WeaponTwo, 
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
	ExitBuildMode(ECharacterState::Standard);
	TimberCharacter->SetCurrentWeaponState(EWeaponState::RangedEquipped);
	WeaponState.Broadcast(EWeaponState::RangedEquipped);

	// Spawning and Attaching the Weapon to the Socket of Right Hand on Leeroy
	 FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = TimberCharacter;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	//Socket Rotation and Location
	const FVector HandSocketLocation = TimberCharacter->GetMesh()->GetSocketLocation("RangedSocket");
	const FRotator HandSocketRotation = TimberCharacter->GetMesh()->GetSocketRotation("RangedSocket");

	FTransform SocketWorldTransform = TimberCharacter->GetMesh()->GetSocketTransform("RangedSocket", ERelativeTransformSpace::RTS_World);
	FVector SocketWorldLocation = SocketWorldTransform.GetLocation();
	FRotator SocketWorldRotation = SocketWorldTransform.Rotator();

	//Spawn the Actor
	ATimberWeaponRangedBase* SpawnedActor = GetWorld()->SpawnActor<ATimberWeaponRangedBase>
	(TimberCharacter->WeaponThree, 
	SocketWorldLocation, SocketWorldRotation, SpawnParams);

	//Attach Actor to the Socket Location
	SpawnedActor->AttachToComponent(TimberCharacter->GetMesh(), 
	FAttachmentTransformRules::SnapToTargetIncludingScale, "RangedSocket");

	//Set the Newly Spawned Weapon to the WeaponOneInstance and CurrentlyEquippedWeapon on Leeroy
	TimberCharacter->WeaponThreeInstance = SpawnedActor;
	TimberCharacter->SetCurrentlyEquippedWeapon(SpawnedActor);
}

void ATimberPlayerController::DisableAllKeyboardInput()
{
	Subsystem->RemoveMappingContext(StandardInputMappingContext);
	Subsystem->RemoveMappingContext(BuildModeInputMappingContext);
}

void ATimberPlayerController::EnableStandardKeyboardInput()
{
	if (Subsystem)
	{
		Subsystem->AddMappingContext(StandardInputMappingContext, 1);
		DisableCursor();
	}
}

void ATimberPlayerController::UnEquipWeapon() const
{
	//TODO:: Play Unequip Animation
	if(TimberCharacter->GetCurrentlyEquippedWeapon())
	{
		//Removing the Currently EquippedWeapon
		TimberCharacter->GetCurrentlyEquippedWeapon()->Destroy();
		//Setting WeaponState on Character
		TimberCharacter->SetCurrentWeaponState(EWeaponState::Unequipped);
		WeaponState.Broadcast(EWeaponState::Unequipped);
	}
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
				//TODO:: Add a punch attack or something here maybe
				GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, "No Weapon Equipped");
			}
			break;
		}
	}

	
}


/*
 * Build System Controls
 */

void ATimberPlayerController::ToggleBuildMode(const FInputActionValue& Value)
{
	
	TimberCharacter->CharacterState == ECharacterState::Building ? TimberCharacter->CharacterState = ECharacterState::Standard : 
	TimberCharacter->CharacterState = ECharacterState::Building;

	// Exiting Build Mode
	if(TimberCharacter->CharacterState == ECharacterState::Standard)
	{
		//WHen leaving building Mode, we need to empty the ActiveBuildingComponent. Why tho? Maybe Unnecessary.
		ExitBuildMode(ECharacterState::Standard);
	}

	// Entering Build Mode
	if (TimberCharacter->CharacterState == ECharacterState::Building)
	{
		OpenBuildModeSelectionMenu();
		if(Subsystem)
		{
			Subsystem->AddMappingContext(BuildModeInputMappingContext, 2);
		}
		UnEquipWeapon();
		
	}
}
//If a player exits build mode with an active building component that isn't placed, destroy it.
void ATimberPlayerController::RemoveBuildingComponentProxy()
{
	
	UBuildSystemManagerComponent* BuildSystemManager = TimberCharacter->BuildSystemManager;
	if(BuildSystemManager)
	{
		ATimberBuildingComponentBase* ActiveComponent = BuildSystemManager->GetActiveBuildingComponent();
  		//Destroy the Active Building Component if it exists.
		if(ActiveComponent) 
		{
			BuildSystemManager->GetActiveBuildingComponent()->Destroy();
		}

		//After utilizing the proxy for visualization, we destroy the proxy and then empty the active build component.
		// This will be set again when ray-casting on a new frame.
		BuildSystemManager->EmptyActiveBuildingComponent();
	}

	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(3, 5.0f, FColor::Green, "Building Component Proxy Removed");
	}
}

void ATimberPlayerController::ExitBuildMode(ECharacterState NewState)
{
	TimberCharacter->CharacterState = NewState;

	CloseBuildModeSelectionMenu();
	DisableCursor();
	
	if(Subsystem)
	{
		// Removing the Buttons used for Build Mode.
		Subsystem->RemoveMappingContext(BuildModeInputMappingContext);
		RemoveBuildingComponentProxy();
	}
	
	TimberCharacter->ExitBuildMode();
}

void ATimberPlayerController::OpenBuildModeSelectionMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("Opening Build Mode Selection Menu Broadcasted"));
	IsBuildPanelOpen.Broadcast(true);
	EnableCursor();
	
}

void ATimberPlayerController::CloseBuildModeSelectionMenu()
{
	IsBuildPanelOpen.Broadcast(false);
	TimberCharacter->BuildSystemManager->SetSavedRotation(FRotator::ZeroRotator);
	bShowMouseCursor = false;
}

void ATimberPlayerController::RotateBuildingComponent(const FInputActionValue& Value)
{
	if(TimberCharacter->CharacterState == ECharacterState::Building && TimberCharacter->BuildSystemManager)
	{
		TimberCharacter->BuildSystemManager->RotateBuildingComponent();
	}
}

void ATimberPlayerController::PlaceBuildingComponent(const FInputActionValue& Value)
{
	
	UBuildSystemManagerComponent* BuildSystemManager = TimberCharacter->BuildSystemManager;
	if(BuildSystemManager)
	{
		BuildSystemManager->SpawnFinalBuildingComponent();
	}
	
}

void ATimberPlayerController::HideBuildMenu(const FInputActionValue& Value)
{
	//Broadcast to HUD to Hide the Build Menu
	ShouldHideBuildMenu.Broadcast();
}

void ATimberPlayerController::DeleteBuildingComponent(const FInputActionValue& Value)
{
	//During Raycast, if we raycast over an existing BC, a UI Widget Will Appear around that Component.

	//Pressing the E button will Delete if Held for 1 Full second.

	if(TimberCharacter->CharacterState == ECharacterState::Building && TimberCharacter->HoveredBuildingComponent)
	{
		//TODO:: Make some kind of Deleted Animation for Building Component so player visually understand the deletion.
		TimberCharacter->HoveredBuildingComponent->Destroy();
		TimberCharacter->HoveredBuildingComponent = nullptr;
	}
}

void ATimberPlayerController::HandlePlayerDeath(bool bIsPlayerDead)
{
	//TODO:: Implement Player Death
	if(bIsPlayerDead)
	{
		
		//TODO:: Play Death Animation
		EnableCursor();
		DisableAllKeyboardInput();
		UnEquipWeapon();

		//Subscribed on the HUD to Show the Death UI
		HandleDeathUI_DelegateHandle.Execute();

		
	}
	
}




