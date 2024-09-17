// Property of Paracosm Industries. Dont use my shit.


#include "Character/TimberPlayableCharacter.h"
#include "BuildSystem/TimberBuildingComponentBase.h"
#include "BuildSystem/TimberBuildSystemManager.h"
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

void ATimberPlayableCharacter::PlayerTakeDamage(float DamageAmount)
{
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

			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(this);
			//CollisionParams.AddIgnoredActor(TSubclassOf<ATimberWeaponBase>);

			FHitResult HitResult;

			bool bHit = GetWorld()->LineTraceSingleByChannel(
				HitResult,
				RaycastStart,
				RaycastEnd,
				ECC_Visibility,
				CollisionParams);
			
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 8, FColor::Red, false, 0.1f);

			if (bHit)
			{
				
				/*Spawn an Active Building Component (ABC) if One Doesn't Exist*/
				ATimberBuildingComponentBase* ActiveBuildingComponent = BuildSystemManagerInstance->GetActiveBuildingComponent();
				if(ActiveBuildingComponent == nullptr || ActiveBuildingComponent->GetClass() != BuildSystemManagerInstance->GetActiveBuildingComponentClass())
				{
					BuildSystemManagerInstance->SpawnBuildingComponent(HitResult.ImpactPoint, GetActorRotation());
				}

				/*If hit actor is ABC, Spawn Delete UI*/
				AActor* HitActor = HitResult.GetComponent()->GetOwner();
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
					
					return;
				}
				else
				{
					//Used once the player moves away from the BuildingComponent
					HoveredBuildingComponent = nullptr;
					//Broadcast a Delegate to the HUD to remove the Delete Icon
					HandleRemoveDeleteIcon_DelegateHandle.Broadcast();
				}

				/*If there is An Active Building Component Move the Proxy to the new location.*/
				if(ActiveBuildingComponent)
				{
					BuildSystemManagerInstance->MoveBuildingComponent(HitResult.ImpactPoint);
				}
					
			}
		}
	}
}

void ATimberPlayableCharacter::HandleBuildMenuOpen(bool bIsBuildMenuOpen)
{
	ShouldRaycast = !bIsBuildMenuOpen;
}
