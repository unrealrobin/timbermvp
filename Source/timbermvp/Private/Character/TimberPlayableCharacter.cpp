// Property of Paracosm Industries. Dont use my shit.


#include "Character/TimberPlayableCharacter.h"

#include "BuildSystem/TimberBuildingComponentBase.h"
#include "BuildSystem/TimberBuildSystemManager.h"
#include "Camera/CameraComponent.h"
#include "Controller/TimberPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

ATimberPlayableCharacter::ATimberPlayableCharacter()
{
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Camera->SetupAttachment(CameraSpringArm);
	CameraSpringArm->SetupAttachment(RootComponent);
}

void ATimberPlayableCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = 800.f;

}

void ATimberPlayableCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	PerformRaycast();
}

void ATimberPlayableCharacter::PerformRaycast()
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
				//Get reference to the BuildSystemManager
				/*ATimberBuildingComponentBase* BuildingComponent = 
				BuildSystemManagerInstance->GetActiveBuildingComponent();

				//If there isn't an Active BuildingComponent, Create One and Set it as Active
				if(!BuildingComponent)
				{
					BuildSystemManagerInstance->SpawnBuildingComponent(HitResult.ImpactPoint, FRotator::ZeroRotator); 
				}
				else //If there is an Active BuildingComponent, Move it to the HitResult.ImpactPoint
				{
					BuildSystemManagerInstance->MoveBuildingComponent(HitResult.ImpactPoint);
				}*/
			}
		}
	}
	
}

void ATimberPlayableCharacter::SetCurrentWeaponState(EWeaponState NewWeaponState)
{
	CurrentWeaponState = NewWeaponState;
}

void ATimberPlayableCharacter::SetCurrentlyEquippedWeapon(ATimberWeaponBase* Weapon)
{
	CurrentlyEquippedWeapon = Weapon;
}




