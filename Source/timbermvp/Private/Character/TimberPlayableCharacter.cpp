// Property of Paracosm Industries. Dont use my shit.


#include "Character/TimberPlayableCharacter.h"
#include "BuildSystem/BuildingComponents/TimberBuildingComponentBase.h"
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
	
}

void ATimberPlayableCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//Initiated From Player Controller Input
	if(CharacterState == ECharacterState::Building && ShouldRaycast)
	{
		PerformBuildSystemRaycast();
	}
}

/*Build System Stuff*/

void ATimberPlayableCharacter::PerformBuildSystemRaycast()
{
	if(CharacterState == ECharacterState::Building)
	{
		ATimberPlayerController* RaycastController = Cast<ATimberPlayerController>(GetController());
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


void ATimberPlayableCharacter::ResetBuildableComponents(TSubclassOf<ABuildableBase> ActiveBuildableClass)
{
	if (ActiveBuildableClass->IsChildOf(ATimberBuildingComponentBase::StaticClass()))
	{
		if(BuildSystemManager->GetActiveBuildingComponent())
		{
			BuildSystemManager->GetActiveBuildingComponent()->Destroy();
			BuildSystemManager->SetActiveBuildingComponentToNull();
		}
	}

	if(ActiveBuildableClass->IsChildOf(ATrapBase::StaticClass()))
	{
		if(BuildSystemManager->GetActiveTrapComponent())
		{
			BuildSystemManager->GetActiveTrapComponent()->Destroy();
			BuildSystemManager->SetActiveTrapComponentToNull();
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
			// This just spawns the trap component proxy if it doesn't exist or if its different then the ActiveBuildableClass
			// This is the first time the trap component is spawned.
			// Hit - Not a Building Component
			// Cant be Final Spawned
			ATrapBase* ActiveTrapComponentProxy = BuildSystemManager->GetActiveTrapComponent();
			if(ActiveTrapComponentProxy == nullptr || ActiveTrapComponentProxy->GetClass()!= BuildSystemManager->GetActiveBuildableClass())
			{
				BuildSystemManager->SpawnTrapComponentProxy(HitResults[0].ImpactPoint, HitResults[0].GetActor()->GetActorRotation());
				if(ActiveTrapComponentProxy)
				{
					ActiveTrapComponentProxy->IsTrapFinalized = false;
				}
			}

			// Getting the first hit Building Component
			ATimberBuildingComponentBase* FirstHitBuildingComponent = nullptr;
			for(const FHitResult& Hits : HitResults)
			{
				if(Cast<ATimberBuildingComponentBase>(Hits.GetActor()))
				{
					FirstHitBuildingComponent = Cast<ATimberBuildingComponentBase>(Hits.GetActor());
					break;
				}
			}

			//Hit a Building Component
			if(FirstHitBuildingComponent)
			{
					BuildSystemManager->MoveBuildingComponent
					(FVector_NetQuantize(FirstHitBuildingComponent->CenterSnap->GetComponentTransform().GetLocation()), 
					ActiveTrapComponentProxy);
			}
			else
			{
				if(ActiveTrapComponentProxy)
				{
					BuildSystemManager->MoveBuildingComponent(HitResults[0].TraceEnd, ActiveTrapComponentProxy);
				}
			}

			//If there is a hit, but it's not a building component, move the trap component around.
			
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
		//No Hits, just don't show anything, clear the variables.
		ResetBuildableComponents(ActiveBuildableClass);
	}
}

bool ATimberPlayableCharacter::HandleBuildingComponentPlacement()
{
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

