// Property of Paracosm Industries. Dont use my shit.


#include "Character/TimberPlayableCharacter.h"
#include "BuildSystem/TimberBuildingComponentBase.h"
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

	if(CharacterState == ECharacterState::Building && ShouldRaycast)
	{
		PerformBuildSystemRaycast();
	}

	
	
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

/*Build System Stuff*/

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
			
			/*
			 * 1st Hit: Quadrant Box Component - HitResult['0']
			 * 2nd Hit: Static Mesh of Building Component Actor - HitResult['1']
			 */

			if (bHits)
			{
				DrawDebugSphere(GetWorld(), HitResults[0].ImpactPoint, 10.f, 8, FColor::Red, false, 0.1f);
				if(HitResults.Num() >= 2)
				{
					//If the second hit is a building component
					if(Cast<ATimberBuildingComponentBase>(HitResults[1].GetActor()))
					{
						BuildSystemManager->HandleBuildingComponentSnapping(HitResults[0], HitResults[1]);
					}

					{
						//HUD Stuff - Delete Widget
						//Needs to be able to get to the actor component during the multicast.
						if (HandleShowDeleteWidget(HitResults[1])) return;
					}
						
				}
				else //Handles the Case where there is no overlap with a Building Component
				{

				/*
				 *Spawn an Active Building Component Proxy(ABCP) if One Doesn't Exist
				 *
				 * Lets move this portion of code into its own function at some point.
				 */
					
					ATimberBuildingComponentBase* ActiveBuildingComponentProxy = BuildSystemManager->GetActiveBuildingComponent();
					if(ActiveBuildingComponentProxy == nullptr || ActiveBuildingComponentProxy->GetClass() != 
					BuildSystemManager->GetActiveBuildingComponentClass())
					{
						BuildSystemManager->SpawnBuildingComponentProxy(HitResults[0].ImpactPoint, GetActorRotation());
					}

					{ //HUD Stuff - Delete Widget
						if (HandleShowDeleteWidget(HitResults[0])) return;
					}

					{
						/*If there is An Active Building Component Move the Proxy to the new location.*/
						if(ActiveBuildingComponentProxy)
						{
							/*Simple Move to Location*/
							//TODO:: Does this need to be deleted after completion of snapping? 
							BuildSystemManager->MoveBuildingComponent(HitResults[0].ImpactPoint);
						}
					}
				}
			}
		}
	}
}

void ATimberPlayableCharacter::HandleBuildMenuOpen(bool bIsBuildMenuOpen)
{
	ShouldRaycast = !bIsBuildMenuOpen;
}
