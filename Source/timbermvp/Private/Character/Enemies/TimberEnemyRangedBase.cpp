// Property of Paracosm Industries. 

#include "Character/Enemies/TimberEnemyRangedBase.h"

#include "Kismet/KismetMathLibrary.h"
#include "Weapons/TimberWeaponBase.h"
#include "Weapons/TimberWeaponRangedBase.h"

// Sets default values
ATimberEnemyRangedBase::ATimberEnemyRangedBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyType = EEnemyType::RangedWeaponRobot;
}

// Called when the game starts or when spawned
void ATimberEnemyRangedBase::BeginPlay()
{
	Super::BeginPlay();
	EquipRangedWeapon();
}

void ATimberEnemyRangedBase::EquipRangedWeapon()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = GetInstigator();

	//Getting the Socket Info from the Mesh
	const FTransform RHandSocketTransform = GetMesh()->GetSocketTransform("RHandRifleSocket");

	if(RangedWeaponClassName != nullptr)
	{
		//Spawning the Class at the Socket Info
		ATimberWeaponBase* RangedWeaponInstance = GetWorld()->SpawnActor<ATimberWeaponBase>(
			RangedWeaponClassName,
			RHandSocketTransform.GetLocation(),
			RHandSocketTransform
			.GetRotation().Rotator(),
			SpawnParameters);

		//Casting the Instance to the Ranged Weapon Class, this will allow spawning of any weapon but only attaching of Ranged Weapon. Maybe change later idk.
		if (RangedWeaponInstance)
		{
			EquippedWeapon = Cast<ATimberWeaponRangedBase>(RangedWeaponInstance);
			if (EquippedWeapon)
			{
				//Attaching the Weapon to the Socket, this locks the weapon to the socket during movement.
				EquippedWeapon->AttachToComponent(
					GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "RHandRifleSocket");
				EnemyWeaponType = EEnemyWeaponState::RangedWeaponEquipped;
			}
		}
	}
}

void ATimberEnemyRangedBase::GetRotationToCurrentTarget()
{
	if (CurrentTarget && EquippedWeapon)
	{
		FVector TargetLocation = CurrentTarget->GetActorLocation();
		FVector StartLocation = EquippedWeapon->GetActorLocation();

		FRotator RotationToTarget = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);
		
		PitchToTarget = RotationToTarget.Pitch;
		YawToTarget = RotationToTarget.Yaw;

		DrawDebugLine(GetWorld(), StartLocation, TargetLocation, FColor::Red, false, 0.1f);

		//DrawDebugSphere(GetWorld(), TargetLocation, 20, 30, FColor::Red);
		//UE_LOG(LogTemp, Warning, TEXT("Ranged Enemy Current Target: %s"), *CurrentTarget->GetName())
	}
}

// Called every frame
void ATimberEnemyRangedBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetMesh())
	{
		if (GetMesh()->GetAnimInstance()->GetActiveMontageInstance())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *GetMesh()->GetName());
		}
	}
}
