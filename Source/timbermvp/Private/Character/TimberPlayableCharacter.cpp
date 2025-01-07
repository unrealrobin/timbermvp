// Property of Paracosm Industries. Dont use my shit.


#include "Character/TimberPlayableCharacter.h"
#include "BuildSystem/BuildingComponents/TimberBuildingComponentBase.h"
#include "BuildSystem/Ramps/RampBase.h"
#include "BuildSystem/Traps/TrapBase.h"
#include "Components/BuildSystem/BuildSystemManagerComponent.h"
#include "Camera/CameraComponent.h"
#include "Controller/TimberPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UI/TimberHUDBase.h"


ATimberPlayableCharacter::ATimberPlayableCharacter()
{
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Camera->SetupAttachment(CameraSpringArm);
	CameraSpringArm->SetupAttachment(RootComponent);
	BuildSystemManager = CreateDefaultSubobject<UBuildSystemManagerComponent>("BuildSystemManager");
}

void ATimberPlayableCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = 800.f;

	/*Delegate Binding*/
	Cast<ATimberHUDBase>(Cast<ATimberPlayerController>(GetController())->GetHUD())->bIsBuildMenuOpen.AddDynamic(this, 
	&ATimberPlayableCharacter::HandleBuildMenuOpen);

	RaycastController = Cast<ATimberPlayerController>(GetController());
	
}

void ATimberPlayableCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//Initiated From Player Controller Input
	if(CharacterState == ECharacterState::Building && ShouldRaycast)
	{
		PerformBuildSystemRaycast();
	}

	//TODO:: This can be made more efficient by only calling this for like 2 seconds after last movement or rotation.
	if(CurrentWeaponState == EWeaponState::RangedEquipped)
	{
		RaycastController->PerformReticleAlignment_Raycast();
	}
}

/*Build System Stuff*/

void ATimberPlayableCharacter::PerformBuildSystemRaycast()
{
	if(CharacterState == ECharacterState::Building)
	{
		if(RaycastController)
		{
			FVector RaycastStart;
			FRotator PlayerRotation;
			Controller->GetPlayerViewPoint(RaycastStart, PlayerRotation);

			//1000 is the range to perform the Raycast.
			FVector RaycastEnd = RaycastStart + (PlayerRotation.Vector() * BuildRaycastDistance);

			/* Ignore the Player Raycasting*/
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(this);
			CollisionParams.AddIgnoredActor(this->CurrentlyEquippedWeapon);

			/* Single Hit*/
			/*FHitResult HitResult;
			bool bHit = GetWorld()->LineTraceSingleByChannel(
				HitResult,
				RaycastStart,
				RaycastEnd,
				ECC_Visibility,
				CollisionParams);*/

			/*Multiple Hits*/
			bool bHits = GetWorld()->LineTraceMultiByChannel(
				HitResults,
				RaycastStart,
				RaycastEnd,
				ECC_Visibility,
				CollisionParams);

			

			HandleRaycastHitConditions(bHits);
		}
	}
}

void ATimberPlayableCharacter::HandleRaycastHitConditions(bool bHits)
{
	TSubclassOf<ABuildableBase> ActiveBuildableClass = BuildSystemManager->GetActiveBuildableClass();
	if (bHits)
	{
		/* Hit Something/Anything */
		DrawDebugSphere(GetWorld(), HitResults[0].ImpactPoint, 10.f, 8, FColor::Red, false, 0.1f);
		
		if(ActiveBuildableClass->IsChildOf(ATrapBase::StaticClass()))
		{
			HandleTrapPlacement();
		}

		if (ActiveBuildableClass->IsChildOf(ARampBase::StaticClass()))
		{
			//TODO:: Call to function in Build System Manager Component to Handle Ramp Spawn and Placement.
			BuildSystemManager->HandleRampPlacement(HitResults);
		}
		
		if (ActiveBuildableClass->IsChildOf(ATimberBuildingComponentBase::StaticClass()))
		{
			// Spawning ActiveBuildingComponent if it doesn't exist or if its different then the ActiveBuildingComponentClass
			ATimberBuildingComponentBase* ActiveBuildingComponentProxy = BuildSystemManager->GetActiveBuildingComponent();
			if(ActiveBuildingComponentProxy == nullptr || ActiveBuildingComponentProxy->GetClass()!= BuildSystemManager->GetActiveBuildableClass())
			{
				BuildSystemManager->SpawnBuildingComponentProxy(HitResults[0].ImpactPoint, GetActorRotation());
			}
			if (HandleBuildingComponentPlacement()) return;
		}
	}
	else
	{
		//TODO:: This can be reduced and simplified 
		BuildSystemManager->ResetBuildableComponents(ATrapBase::StaticClass());
		BuildSystemManager->ResetBuildableComponents(ATimberBuildingComponentBase::StaticClass());
		BuildSystemManager->ResetBuildableComponents(ARampBase::StaticClass());
	}
}

bool ATimberPlayableCharacter::HandleBuildingComponentPlacement()
{
	//TODO:: Change this to a more dynamic way of finding the first hit Building Component and Quadrant. Similar to the approach in HandleTrapPlacement.
	//Handle Building Component Placement
	//Hit Result is Stored in Global Scope of the Player Character
	if(HitResults.Num() >= 2)
	{
		//If the second hit is a building component, snap to that building component utilizing the quadrant system.
		// Using the 2nd Hit because the first visible hit is the Quadrant Box Component.
		/* Hit a Building Component Condition */
		if(Cast<ATimberBuildingComponentBase>(HitResults[1].GetActor()))
		{
			BuildSystemManager->HandleBuildingComponentSnapping(HitResults[0], HitResults[1]);
		}

		//TODO:: Can we move this anywhere that will work for both traps and building components 
		//HUD Stuff - Delete Widget
		//Needs to be able to get to the actor component during the multicast.
		if (HandleShowDeleteWidget(HitResults[1])) return true;
		
						
	}
	else //Handles the Case where there is no overlap with a Building Component and Moves the Building Component Around
	{
		//HUD Stuff - Delete Widget
		if (HandleShowDeleteWidget(HitResults[0])) return true;
		
		/*If there is An Active Building Component Move the Proxy to the new location.*/
		if(BuildSystemManager->GetActiveBuildingComponent())
		{
			/*Simple Move to Location*/
			BuildSystemManager->MoveBuildingComponent(HitResults[0].ImpactPoint, BuildSystemManager->GetActiveBuildingComponent());
		}
		
	}
	return false;
}

void ATimberPlayableCharacter::HandleTrapPlacement()
{
	//Just to get Here the Raycast must have hit something.
	
	//SPAWNING TRAP COMPONENT
	ATrapBase* ActiveTrapComponentProxy = BuildSystemManager->GetActiveTrapComponent();
	if(ActiveTrapComponentProxy == nullptr || ActiveTrapComponentProxy->GetClass()!= BuildSystemManager->GetActiveBuildableClass())
	{
		BuildSystemManager->SpawnTrapComponentProxy(HitResults[0].ImpactPoint, HitResults[0].GetActor()->GetActorRotation());
		
		if(ActiveTrapComponentProxy)
		{
			ActiveTrapComponentProxy->SetCanTrapBeFinalized(false);
		}
	}
	
	// LOOKING FOR HITS ON A BUILDING COMPONENT
	// Search all hits of Multi Ray Cast for one the first that casts to a BuildingComponentBase (Ramp || Wall )
	ATimberBuildingComponentBase* FirstHitBuildingComponent = nullptr;
	for(const FHitResult& Hits : HitResults)
	{
		if(Cast<ATimberBuildingComponentBase>(Hits.GetActor()))
		{
			FirstHitBuildingComponent = Cast<ATimberBuildingComponentBase>(Hits.GetActor());
			BuildingComponentImpactPoint = Hits.ImpactPoint;
			break;
		}
	}
	
	//HIT A BUILDING COMPONENT
	if(FirstHitBuildingComponent)
	{
		// Pairing the trap with the wall its Hovering over.
		if(ActiveTrapComponentProxy)
		{
			ActiveTrapComponentProxy->HoveredBuildingComponent = FirstHitBuildingComponent;

			FTrapSnapData TrapSnapData = BuildSystemManager->GetTrapSnapTransform(BuildingComponentImpactPoint,
																			  FirstHitBuildingComponent, BuildSystemManager->GetActiveTrapComponent());
			BuildSystemManager->MoveBuildingComponent
				(FVector_NetQuantize(TrapSnapData.TrapLocation), 
				 ActiveTrapComponentProxy,
				 TrapSnapData.TrapRotation);
		}
		
	}
	else // HIT BUT NOT A BUILDING COMPONENT
	{
		if(ActiveTrapComponentProxy)
		{
			ActiveTrapComponentProxy->SetCanTrapBeFinalized(false);
			ActiveTrapComponentProxy->HoveredBuildingComponent = nullptr;
			if(HitResults[0].ImpactPoint != FVector::ZeroVector)
			{
				FRotator PlayerRotation = GetActorTransform().GetRotation().Rotator();
				PlayerRotation.Yaw = PlayerRotation.Yaw - 180;
				BuildSystemManager->MoveBuildingComponent(HitResults[0].ImpactPoint, ActiveTrapComponentProxy, 
				PlayerRotation);
			}
		}
		
		
	}
}

bool ATimberPlayableCharacter::HandleShowDeleteWidget(FHitResult HitResult)
{
	//TODO:: Can we somehow get an array of hitresult from the RaycastMulti and check if the array contrains a Building Component?
	
	AActor* HitActor = HitResult.GetComponent()->GetOwner(); //Hit can be one of the quadrants so we want to make sure that the hit is a Building Component
	if(Cast<ATimberBuildingComponentBase>(HitActor)) 
	{
		HoveredBuildingComponent = Cast<ATimberBuildingComponentBase>(HitActor);
		if(HoveredBuildingComponent)
		{
			//Translating a Location in World Space to Its Location in Screen Space
			FVector2d ScreenLocationOfImpactPoint;
			GetWorld()->GetFirstPlayerController()->ProjectWorldLocationToScreen(HitResult.ImpactPoint, 
				ScreenLocationOfImpactPoint);
						
			// Broadcast a Delegate with the Impact Position to the HUD.
			HandleSpawnDeleteIconLocation_DelegateHandle.Broadcast(ScreenLocationOfImpactPoint.X,ScreenLocationOfImpactPoint.Y);
		}
					
		return true;
	}
	else if(Cast<ATimberBuildingComponentBase>(HitResult.GetActor()))
	{
		HoveredBuildingComponent = Cast<ATimberBuildingComponentBase>(HitResult.GetActor());
		if(HoveredBuildingComponent)
		{
			//Translating a Location in World Space to Its Location in Screen Space
			FVector2d ScreenLocationOfImpactPoint;
			GetWorld()->GetFirstPlayerController()->ProjectWorldLocationToScreen(HitResult.ImpactPoint, 
				ScreenLocationOfImpactPoint);
						
			// Broadcast a Delegate with the Impact Position to the HUD.
			HandleSpawnDeleteIconLocation_DelegateHandle.Broadcast(ScreenLocationOfImpactPoint.X,ScreenLocationOfImpactPoint.Y);
		}
	}
	else
	{
		ExitBuildMode();

		return false;
	}


	return false;
}

void ATimberPlayableCharacter::HandleBuildMenuOpen(bool bIsBuildMenuOpen)
{
	ShouldRaycast = !bIsBuildMenuOpen;
}

/*Death & Damage*/

void ATimberPlayableCharacter::PlayDeathAnimation()
{
	PlayAnimMontage(DeathMontage, 1.f, FName("Death1"));
}

void ATimberPlayableCharacter::HandlePlayerDeath()
{
	if(bIsPlayerDead)
	{
		PlayDeathAnimation();
		//Broadcasting the Player Death Delegate
		//Player Controller is Subscribed to this Delegate
		HandlePlayerDeath_DelegateHandle.Broadcast(bIsPlayerDead);
	}
}

void ATimberPlayableCharacter::ExitBuildMode()
{
	BuildSystemManager->RemoveBuildingComponentProxies_All();
	HoveredBuildingComponent = nullptr;
	HandleRemoveDeleteIcon_DelegateHandle.Broadcast();
}

void ATimberPlayableCharacter::PlayerTakeDamage(float DamageAmount)
{	
	//TODO:: Should character have any defensive abilities that reduce damage amount, calculate here before applying damage.
	
	CurrentHealth -= DamageAmount;
	if(CurrentHealth <= 0.f)
	{
		bIsPlayerDead = true;
		HandlePlayerDeath();
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Hit for: %f. CurrentHealth: %f."), DamageAmount, CurrentHealth);
	}
}

/*Weapon Stuff*/

void ATimberPlayableCharacter::SetCurrentWeaponState(EWeaponState NewWeaponState)
{
	CurrentWeaponState = NewWeaponState;
}

void ATimberPlayableCharacter::SetCurrentlyEquippedWeapon(ATimberWeaponBase* Weapon)
{
	CurrentlyEquippedWeapon = Weapon;
}

