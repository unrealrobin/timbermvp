// Property of Paracosm Industries. Dont use my shit.


#include "Weapons/TimberWeaponRangedBase.h"
#include "Character/TimberCharacterBase.h"
#include "Controller/TimberPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Weapons/Projectiles/TimberProjectileBase.h"

// Sets default values
ATimberWeaponRangedBase::ATimberWeaponRangedBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileSpawnComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnComponent"));
	ProjectileSpawnComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ATimberWeaponRangedBase::BeginPlay()
{
	Super::BeginPlay();

	//Set in Spawn Params
	WeaponOwner = GetOwner();
}

// Called every frame
void ATimberWeaponRangedBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATimberWeaponRangedBase::FireRangedWeapon(FVector TargetLocation)
{
	if (WeaponOwner && GetWorld())
	{
		if (ProjectileType)
		{
			FVector ProjectileSpawnLocation = ProjectileSpawnComponent->GetComponentLocation();
			//FRotator ControllerDirection = Cast<ATimberCharacterBase>(WeaponOwner)->GetController()->GetControlRotation();

			//This is the updated Rotation based on the raycast using the HitResult from the Screen Space. (More Accurate.)
			FRotator AimRotation = (TargetLocation - ProjectileSpawnLocation).Rotation();

			// Add Spawn Params for Projectile Owner and Instigator
			FActorSpawnParameters SpawnParams;
			// Print the owner of the weapon
			//UE_LOG(LogTemp, Warning, TEXT("Player Weapon Owner: %s"), *GetOwner()->GetName());
			SpawnParams.Owner = WeaponOwner;

			ATimberProjectileBase* Projectile = GetWorld()->SpawnActor<ATimberProjectileBase>(
				ProjectileType,
				ProjectileSpawnLocation, AimRotation, SpawnParams);

			if (Projectile)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), FiringSound, ProjectileSpawnLocation);
				Projectile->SetOwner(this);
				GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::Green, "Projectile Spawned.");
			}
		}
	}
}

void ATimberWeaponRangedBase::AI_FireRangedWeapon()
{
	if (WeaponOwner && GetWorld())
	{
		if (ProjectileType)
		{
			GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::Green, "Projectile All Variables Loaded");
			FVector ProjectileSpawnLocation = ProjectileSpawnComponent->GetComponentLocation();

			//Get Player Character
			FRotator ControllerDirection = Cast<ATimberCharacterBase>(WeaponOwner)->GetController()->
				GetControlRotation();
			FRotator RandomAimOffset = FRotator(
				FMath::RandRange(-3, 3), FMath::RandRange(-3, 3), FMath::RandRange(-3, 3));
			ATimberProjectileBase* Projectile = GetWorld()->SpawnActor<ATimberProjectileBase>(
				ProjectileType, ProjectileSpawnLocation, ControllerDirection + RandomAimOffset);

			if (Projectile)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FiringSound, ProjectileSpawnLocation);
				Projectile->SetOwner(this);
				GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::Green, "Projectile Spawned.");
			}
		}
	}
}

void ATimberWeaponRangedBase::PerformStandardAttack()
{
}
