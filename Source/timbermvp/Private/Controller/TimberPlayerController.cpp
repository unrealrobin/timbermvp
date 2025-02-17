// Property of Paracosm Industries. Dont use my shit.


#include "Controller/TimberPlayerController.h"
#include "Interfaces/Interactable.h"
#include "EnhancedInputSubsystems.h"
#include "UI/BuildingComponent.h"
#include "BuildSystem/BuildingComponents/TimberBuildingComponentBase.h"
#include "Weapons/TimberWeaponMeleeBase.h"
#include "Character/TimberPlayableCharacter.h"
#include "Components/BuildSystem/BuildSystemManagerComponent.h"
#include "Data/DataAssets/BuildComponentDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameModes/TimberGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/TimberHUDBase.h"

#include "Weapons/TimberWeaponBase.h"
#include "Weapons/TimberWeaponRangedBase.h"

void ATimberPlayerController::BeginPlay()
{
	Super::BeginPlay();


	{
	/*//TODO:: How can I make this better? Seems rigid. This can change mid game based on colliding objects, build locations, etc.
		// This is used for the Saving and Loading Process. We need a Location in space to respawn during load.
		APlayerStart* PlayerStartObject = Cast<APlayerStart>(
			UGameplayStatics::GetActorOfClass(
				GetWorld(),
				APlayerStart::StaticClass()));
		PlayerStartLocation = PlayerStartObject->GetActorLocation();*/
	}

	Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());

	TimberCharacter = Cast<ATimberPlayableCharacter>(GetPawn());
	TimberCharacterSpringArmComponent = TimberCharacter->GetSpringArmComponent();
	TimberCharacterMovementComponent = TimberCharacter->GetCharacterMovement();
	EnableStandardKeyboardInput();
	
	{
		//When switching from Game Start Menu to Level, we switch back the Input Mode to Game and UI
		FInputModeGameAndUI GameAndUIInputMode;
		FInputModeGameOnly GameOnlyInputMode;
		SetInputMode(GameOnlyInputMode);
	}

	//Delegate Subscription
	TimberCharacter->HandlePlayerDeath_DelegateHandle.AddDynamic(this, &ATimberPlayerController::HandlePlayerDeath);

	ATimberGameModeBase* GameMode = Cast<ATimberGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
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
	EnhancedInputComponent->BindAction(
		MoveAction, ETriggerEvent::Completed, this, &ATimberPlayerController::MoveComplete);
	EnhancedInputComponent->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::LookUp);
	EnhancedInputComponent->BindAction(
		LookRightAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::LookRight);
	EnhancedInputComponent->BindAction(
		JumpAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::CharacterJump);
	EnhancedInputComponent->BindAction(
		InteractAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::Interact);
	EnhancedInputComponent->BindAction(
		EquipWeaponOneAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::EquipWeaponOne);
	EnhancedInputComponent->BindAction(
		EquipWeaponTwoAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::EquipWeaponTwo);
	EnhancedInputComponent->BindAction(
		EquipWeaponThreeAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::EquipWeaponThree);
	EnhancedInputComponent->BindAction(
		StandardAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::StandardAttack);
	EnhancedInputComponent->BindAction(
		ToggleBuildModeAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::EnterBuildMode);
	EnhancedInputComponent->BindAction(
		RotateBuildingComponentAction, ETriggerEvent::Triggered, this,
		&ATimberPlayerController::RotateBuildingComponent);
	EnhancedInputComponent->BindAction(
		PlaceBuildingComponentAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::PlaceBuildingComponent);
	EnhancedInputComponent->BindAction(
		HideBuildMenuAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::HideBuildMenu);
	EnhancedInputComponent->BindAction(
		DeleteBuildingComponentAction, ETriggerEvent::Triggered, this,
		&ATimberPlayerController::DeleteBuildingComponent);
	EnhancedInputComponent->BindAction(ModifyCursorAction_Controller, ETriggerEvent::Triggered, this, &ATimberPlayerController::ModifyCursorWithController);
	EnhancedInputComponent->BindAction(SelectIconAction_Controller, ETriggerEvent::Triggered, this, &ATimberPlayerController::SelectBCIcon_Controller);
	EnhancedInputComponent->BindAction(ReloadWeaponInputAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::ReloadWeapon);
	EnhancedInputComponent->BindAction(ExitBuildModeAction, ETriggerEvent::Triggered, this, &ATimberPlayerController::ExitBuildMode);
}

void ATimberPlayerController::PerformReticuleAlignment_Raycast()
{
	/*
	 * Performs Raycast from the camera to the center of the screen and aligns the reticule to the hit location.
	 */
	FVector CameraLocation;
	FVector CameraDirection;

	int ViewportSizeX;
	int ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	FVector2d ScreenCenter(ViewportSizeX * 0.5f, ViewportSizeY * 0.5f);

	/*
	 * Camera is the screen as you see it.
	 * Line Trace goes from the center of the screen (where reticule should be) out to the world by X (10,000.f) units.
	 * Expensive, but that's why we only want 1 hit result.
	 */
	if (DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, CameraLocation, CameraDirection))
	{
		if (CameraDirection.Normalize())
		{
			FHitResult HitResult;
			FVector End = CameraLocation + (CameraDirection * 10000.f);
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(GetPawn());
			QueryParams.AddIgnoredActor(TimberCharacter->GetCurrentlyEquippedWeapon());
			if (GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, End, ECC_Visibility))
			{
				ReticuleHitLocation = HitResult.ImpactPoint;
			}
			else
			{
				ReticuleHitLocation = End;
			}
		}
	}
}

void ATimberPlayerController::EnableCursor()
{
	bShowMouseCursor = true;
	USpringArmComponent* CharacterSpringArm = Cast<USpringArmComponent>(
		TimberCharacter->GetComponentByClass(USpringArmComponent::StaticClass()));
	CharacterSpringArm->bUsePawnControlRotation = false;
}

void ATimberPlayerController::DisableCursor()
{
	bShowMouseCursor = false;
	USpringArmComponent* CharacterSpringArm = Cast<USpringArmComponent>(
		TimberCharacter->GetComponentByClass(USpringArmComponent::StaticClass()));
	CharacterSpringArm->bUsePawnControlRotation = true;
}

void ATimberPlayerController::SetInteractableItem(IInteractable* Item)
{
	InteractableItem = Item;
}

void ATimberPlayerController::ClearInteractableItem()
{
	InteractableItem = nullptr;
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
		FRotator NewRotation = FMath::RInterpTo(
			ControlledPawn->GetActorRotation(), TargetForwardDirection, GetWorld()->GetDeltaSeconds(), 5.f);
		ControlledPawn->SetActorRotation(NewRotation);

		//Adding Movement Input to the Character
		ControlledPawn->AddMovementInput(ControllerForwardVector, Value.Get<FVector2D>().X, false);
		ControlledPawn->AddMovementInput(CharacterRightMoveDirection, Value.Get<FVector2D>().Y, false);
	}
}

void ATimberPlayerController::MoveComplete(const FInputActionValue& Value)
{
	//This just resets the MoveInputActionValue to 0,0 after the last input is released. Otherwise the character will keep moving in the last direction.
	MoveInputActionValue = FVector2d(0.f, 0.f);
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

	if (IsFalling)
	{
		CanJump = false;
	}

	if (bIsMovingOnGround || bIsIdle)
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
	TimberCharacter->IsNowJumping = false;
	//Is the Bool we use to Switch between Animations Tracks for Blending Purposes.
	SwitchToWalking = true;
	CanJump = false;
}

void ATimberPlayerController::CharacterJump(const FInputActionValue& Value)
{
	CanCharacterJump(); // sets the CanJump Variable
	SwitchToWalking = false;
	if (CanJump)
	{
		TimberCharacter->IsNowJumping = true;
		//Is the Bool we use to Switch between Animations Tracks for Blending Purposes.
		if (TimberCharacter->IsNowJumping)
		{
			TimberCharacter->Jump();
		}
	}
}

void ATimberPlayerController::Interact(const FInputActionValue& Value)
{
	//TODO:: Need to implement some kind of check if overlapping multiple interactable items.
	
	if (Value.Get<bool>() && InteractableItem)
	{
		if (InteractableItem)
		{
			InteractableItem->Interact();
		}
	}
}

//Sword
void ATimberPlayerController::EquipWeaponOne(const FInputActionValue& Value)
{
	if (TimberCharacter)
	{
		UnEquipWeapon();
		HandleExitBuildMode();
		//Setting WeaponState on Character
		TimberCharacter->SetCurrentWeaponState(EWeaponState::AxeEquipped);
		WeaponState.Broadcast(EWeaponState::AxeEquipped);

		// Spawning and Attaching the Weapon to the Socket of Right Hand on Leeroy
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Cast<ATimberPlayableCharacter>(GetPawn());
		SpawnParams.Instigator = GetPawn();

		//Socket Rotation and Location
		const FVector HandSocketLocation = TimberCharacter->GetMesh()->GetSocketLocation("AxeSocket");
		const FRotator HandSocketRotation = TimberCharacter->GetMesh()->GetSocketRotation("AxeSocket");

		FTransform SocketWorldTransform = TimberCharacter->GetMesh()->GetSocketTransform("AxeSocket", RTS_World);
		FVector SocketWorldLocation = SocketWorldTransform.GetLocation();
		FRotator SocketWorldRotation = SocketWorldTransform.Rotator();


		//Spawn the Weapon
		ATimberWeaponMeleeBase* SpawnedActor = GetWorld()->SpawnActor<ATimberWeaponMeleeBase>
		(
			TimberCharacter->WeaponOne,
			SocketWorldLocation, SocketWorldRotation, SpawnParams);

		//Attach Actor to the Socket Location
		SpawnedActor->AttachToComponent(
			TimberCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "AxeSocket");

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

	HandleExitBuildMode();
	TimberCharacter->SetCurrentWeaponState(EWeaponState::ChainsawEquipped);
	WeaponState.Broadcast(EWeaponState::ChainsawEquipped);
	// Spawning and Attaching the Weapon to the Socket of Right Hand on Leeroy
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Cast<ATimberPlayableCharacter>(GetPawn());
	SpawnParams.Instigator = GetPawn();  
	//Socket Rotation and Location
	const FVector HandSocketLocation = TimberCharacter->GetMesh()->GetSocketLocation("ChainSawSocket");
	const FRotator HandSocketRotation = TimberCharacter->GetMesh()->GetSocketRotation("ChainSawSocket");

	FTransform SocketWorldTransform = TimberCharacter->GetMesh()->GetSocketTransform("ChainSawSocket", RTS_World);
	FVector SocketWorldLocation = SocketWorldTransform.GetLocation();
	FRotator SocketWorldRotation = SocketWorldTransform.Rotator();

	//Spawn the Actor
	ATimberWeaponMeleeBase* SpawnedActor = GetWorld()->SpawnActor<ATimberWeaponMeleeBase>(
		TimberCharacter->WeaponTwo,
		SocketWorldLocation, SocketWorldRotation, SpawnParams);

	//Attach Actor to the Socket Location
	SpawnedActor->AttachToComponent(
		TimberCharacter->GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale, "ChainSawSocket");

	//Set the Newly Spawned Weapon to the WeaponOneInstance and CurrentlyEquippedWeapon on Leeroy
	TimberCharacter->WeaponTwoInstance = SpawnedActor;
	TimberCharacter->SetCurrentlyEquippedWeapon(SpawnedActor);
	//Set Leeroy on the Owner of the Weapon so we can Reference the Owner from the Weapon.
	SpawnedActor->SetOwner(TimberCharacter);
}

//Rifle
void ATimberPlayerController::EquipWeaponThree(const FInputActionValue& Value)
{
	check(TimberCharacter);

	UnEquipWeapon();

	HandleExitBuildMode();
	
	TimberCharacter->SetCurrentWeaponState(EWeaponState::RangedEquipped);
	WeaponState.Broadcast(EWeaponState::RangedEquipped);

	// Spawning and Attaching the Weapon to the Socket of Right Hand on Leeroy
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Cast<ATimberPlayableCharacter>(GetPawn());
	SpawnParams.Instigator = GetPawn();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	//Socket Rotation and Location
	const FVector HandSocketLocation = TimberCharacter->GetMesh()->GetSocketLocation("RangedSocket");
	const FRotator HandSocketRotation = TimberCharacter->GetMesh()->GetSocketRotation("RangedSocket");

	FTransform SocketWorldTransform = TimberCharacter->GetMesh()->GetSocketTransform("RangedSocket", RTS_World);
	FVector SocketWorldLocation = SocketWorldTransform.GetLocation();
	FRotator SocketWorldRotation = SocketWorldTransform.Rotator();

	//Spawn the Actor
	ATimberWeaponRangedBase* SpawnedActor = GetWorld()->SpawnActor<ATimberWeaponRangedBase>
	(
		TimberCharacter->WeaponThree,
		SocketWorldLocation, SocketWorldRotation, SpawnParams);

	//Attach Actor to the Socket Location
	if (SpawnedActor)
	{
		SpawnedActor->AttachToComponent(
			TimberCharacter->GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale, "RangedSocket");

		//Set the Newly Spawned Weapon to the WeaponOneInstance and CurrentlyEquippedWeapon on Leeroy
		TimberCharacter->WeaponThreeInstance = SpawnedActor;
		TimberCharacter->SetCurrentlyEquippedWeapon(SpawnedActor);

		//Telling HUD to Display the Ammo Counter when the Ranged Weapon is Equipped
		ShowAmmoCounter.Broadcast(true);
	}
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
	if (TimberCharacter->GetCurrentlyEquippedWeapon())
	{
		//Broadcasts to HUD if Unequipped Weapon is the Ranged Weapon (Hides Ammo Counter)
		HandleWeaponEquip();
		
		//Removing the Currently EquippedWeapon\]
		//TODO:: Can we modify this to not destroy the Weapon but HIDE it?
		TimberCharacter->GetCurrentlyEquippedWeapon()->Destroy();

		//Stops any reloading Montages or shooting montages from playing - May also stop death montage if we switch weapons on death.
		TimberCharacter->StopAllAnimMontages();
		
		//Setting WeaponState on Character
		TimberCharacter->SetCurrentWeaponState(EWeaponState::Unequipped);
		
		WeaponState.Broadcast(EWeaponState::Unequipped);
	}
}

void ATimberPlayerController::HandleWeaponEquip() const
{
	//If Unequipped the Ranged Weapon, we need to Hide the Ammo Counter
	if (Cast<ATimberWeaponRangedBase>(TimberCharacter->GetCurrentlyEquippedWeapon()))
	{
		ShowAmmoCounter.Broadcast(false);
	}
}

void ATimberPlayerController::StandardAttack(const FInputActionValue& Value)
{
	if (TimberCharacter && TimberCharacter->GetCurrentWeaponState() != EWeaponState::Unequipped && CanAttackAgain)
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
				TimberCharacter->WeaponThreeInstance->FireRangedWeapon(ReticuleHitLocation);
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

void ATimberPlayerController::EnterBuildMode(const FInputActionValue& Value)
{
	//Changing Character State to Build Mode.
	TimberCharacter->CharacterState = ECharacterState::Building;

	// Entering Build Mode
	if (TimberCharacter->CharacterState == ECharacterState::Building)
	{
		if (Subsystem)
		{
			//Adding IMC for BuildMode - OverWrites on the overwritten Standard IMC
			Subsystem->AddMappingContext(BuildModeInputMappingContext, 2);
		}
		
		UnEquipWeapon();
		EnableCursor();

		//Deletes Lingering Proxies
		TimberCharacter->BuildSystemManager->RemoveBuildingComponentProxies_All();
		
		//HUD Responsibility
		OpenBuildModeSelectionMenu();
	}
}

void ATimberPlayerController::OpenBuildModeSelectionMenu()
{
	//Broadcast to the HUD to Open the Build Menu
	IsBuildPanelOpen.Broadcast(true);
	
}

void ATimberPlayerController::HandleExitBuildMode()
{
	//Exiting Build Mode
	if (TimberCharacter->CharacterState == ECharacterState::Building)
	{
		//Handles changes on the Controller when Leaving Build Mode.
		HandleControllerExitBuildMode();

		//Broadcast to HUD to Hide the Build Menu - Handles Cursor Changes.
		ShouldHideBuildMenu.Broadcast();

		FlushPressedKeys();
	}
}

void ATimberPlayerController::ExitBuildMode(const FInputActionValue& Value)
{
	/*This only works when the Build Mode Input Mapping Context is Active
	 * all keymappings here won't work when the Standard Input Mapping Context is Active.
	 */
	
	HandleExitBuildMode();
}

void ATimberPlayerController::HandleControllerExitBuildMode()
{
	TimberCharacter->CharacterState = ECharacterState::Standard;

	DisableCursor();

	if (Subsystem)
	{
		// Removing the Buttons used for Build Mode.
		Subsystem->RemoveMappingContext(BuildModeInputMappingContext);
	}

	TimberCharacter->ResetDeleteIcon();

	//Deletes Lingering Proxies
	TimberCharacter->BuildSystemManager->RemoveBuildingComponentProxies_All();
}

void ATimberPlayerController::RotateBuildingComponent(const FInputActionValue& Value)
{
	if (TimberCharacter->CharacterState == ECharacterState::Building && TimberCharacter->BuildSystemManager)
	{
		TimberCharacter->BuildSystemManager->RotateBuildingComponent();
	}
}

void ATimberPlayerController::PlaceBuildingComponent(const FInputActionValue& Value)
{
	UBuildSystemManagerComponent* BuildSystemManager = TimberCharacter->BuildSystemManager;
	if (BuildSystemManager)
	{
		BuildSystemManager->SpawnFinalBuildable();
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

	if (TimberCharacter->CharacterState == ECharacterState::Building && TimberCharacter->HoveredBuildingComponent)
	{
		//When the Buildable is Deleted by the Player, it will redrop the cost of the buildable.
		TimberCharacter->HoveredBuildingComponent->HandleDeletionOfBuildable();
		TimberCharacter->HoveredBuildingComponent = nullptr;
	}
}

void ATimberPlayerController::HandlePlayerDeath(bool bIsPlayerDead)
{
	
	if (bIsPlayerDead)
	{
		EnableCursor();
		DisableAllKeyboardInput();
		UnEquipWeapon();

		//Subscribed on the HUD to Show the Death UI
		HandleDeathUI_DelegateHandle.Execute();
	}
}

/* Controller Only */
void ATimberPlayerController::ModifyCursorWithController(const FInputActionValue& Value)
{
	// Get the current mouse position
	FVector2D CurrentMousePos;
	GetMousePosition(CurrentMousePos.X, CurrentMousePos.Y);

	// Get the joystick input value
	FVector2D AnalogValue = Value.Get<FVector2D>();

	// Delta time for frame-independent movement
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	// Define cursor speed (you can make this a class variable for tuning)
	float CursorSpeed = 800.0f;

	// Calculate the new mouse position
	FVector2D NewMousePos = CurrentMousePos + (AnalogValue * CursorSpeed * DeltaTime);
	DrawDebugPoint(GetWorld(), FVector(NewMousePos, 0), 5, FColor::Red, false, 1.0f);
	
	// Update the mouse location
	SetMouseLocation(NewMousePos.X, NewMousePos.Y);
}

void ATimberPlayerController::SelectBCIcon_Controller(const FInputActionValue& Value)
{
	/*
	 * Hover focuses the widget.
	 * Store the Widget in a variable on the controller.
	 * Retrieve the Data Asset Stored on the Widget.
	 * Get the BP Class Name Stored on the Data Asset.
	 * Set that to the ActiveBuildableComponentClass.
	 * Close the Build Menu Panel. HUD::ShouldHideBuildMenu 
	 */
			if(HoveredIconDataAsset)
			{
				TSubclassOf<ABuildableBase> BuildingComponentClassName = HoveredIconDataAsset->BuildingComponentClass;
				UE_LOG(LogTemp, Warning, TEXT("THE FUCKIN CLASS NAME: %s"), *BuildingComponentClassName->GetName());
				if(TimberCharacter)
				{
					TimberCharacter->BuildSystemManager->SetActiveBuildingComponentClass(BuildingComponentClassName);
					ATimberHUDBase* HUD = Cast<ATimberHUDBase>(GetHUD());

					//Close the Build Menu Panel (Doesn't Leave Build State, Same Effect as TAB)
					HUD->CloseBuildPanelMenu();

					//Clear Focused Widget
					FocusedWidget = nullptr;
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No data asset found on Building Component Icon Widget"));
			}
	
}

void ATimberPlayerController::ReloadWeapon(const FInputActionValue& Value)
{
	if (TimberCharacter->GetCurrentWeaponState() ==  EWeaponState::RangedEquipped
		&& TimberCharacter->CharacterState == ECharacterState::Standard)
	{
		//TODO:: Play Reload Animation Montage - Set Notify to call the reload function at end of animation. 
		TimberCharacter->WeaponThreeInstance->PlayReloadMontage();
	}
	
}


