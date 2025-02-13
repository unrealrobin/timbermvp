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

	//Setting Ammo
	CurrentAmmo = MaxAmmo;
}

void ATimberWeaponRangedBase::ResetFiringCooldown()
{
	GetWorld()->GetTimerManager().ClearTimer(TimeBetweenShotsHandle);
	bIsFireOnCooldown = false;
}

// Called every frame
void ATimberWeaponRangedBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATimberWeaponRangedBase::FireRangedWeapon(FVector TargetLocation)
{
	if (bIsFireOnCooldown == false && CurrentAmmo > 0)
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
					//Setting Timer to next shot
					GetWorld()->GetTimerManager().SetTimer(TimeBetweenShotsHandle, this, 
					&ATimberWeaponRangedBase::ResetFiringCooldown, TimeBetweenProjectiles, false);

					//Setting Firing to on Cooldown
					bIsFireOnCooldown = true;

					//Removing Ammo from Available Ammo
					CurrentAmmo--;

					//Playing firing sounds
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), FiringSound, ProjectileSpawnLocation);
					
					Projectile->SetOwner(this);
					
					GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::Green, "Projectile Spawned.");
				}
			}
		}
	}
}

void ATimberWeaponRangedBase::AI_FireRangedWeapon()
{
	if (bIsFireOnCooldown == false)
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
					FMath::RandRange(-1, 1), FMath::RandRange(-1, 1), FMath::RandRange(-1, 1));
				ATimberProjectileBase* Projectile = GetWorld()->SpawnActor<ATimberProjectileBase>(
					ProjectileType, ProjectileSpawnLocation, ControllerDirection + RandomAimOffset);

				if (Projectile)
				{
					//Cooldown for AI Automatic Fire.
					GetWorld()->GetTimerManager().SetTimer(TimeBetweenShotsHandle, this, &ATimberWeaponRangedBase::ResetFiringCooldown, TimeBetweenProjectiles, false);
					bIsFireOnCooldown = true;
					
					UGameplayStatics::PlaySoundAtLocation(this, FiringSound, ProjectileSpawnLocation);
					
					Projectile->SetOwner(this);
					
					GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::Green, "Projectile Spawned.");
				}
			}
		}
	}
}

void ATimberWeaponRangedBase::PerformStandardAttack()
{
}

void ATimberWeaponRangedBase::PlayReloadMontage()
{
	ATimberPlayableCharacter* PlayerCharacter = Cast<ATimberPlayableCharacter>(WeaponOwner);
	if (PlayerCharacter && PlayerCharacter->ReloadMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ranged Weapon - PlayReloadMontage() - PlayerCharacter is Owner - Reloading Weapon"));
		//Plays the montage. Notify on Montage end handles the Reload.
		PlayerCharacter->PlayAnimMontage(PlayerCharacter->ReloadMontage, 1.f, FName("Reload1"));
	}
}

void ATimberWeaponRangedBase::ReloadWeapon()
{
	CurrentAmmo = MaxAmmo;
}
