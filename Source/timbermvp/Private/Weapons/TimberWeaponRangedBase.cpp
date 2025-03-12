// Property of Paracosm Industries. Dont use my shit.


#include "Weapons/TimberWeaponRangedBase.h"
#include "Character/TimberCharacterBase.h"
#include "Controller/TimberPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Weapons/Projectiles/TimberPlayerProjectile.h"
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
	if (bIsFireOnCooldown == false && CurrentAmmo > 0 && bIsReloading == false)
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
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				//UE_LOG(LogTemp, Warning, TEXT("Attempting to Spawn Projectile."));

		
				//Deffering spawn to set all ownership first.		
				FTransform ProjectileSpawnTransform = FTransform(AimRotation, ProjectileSpawnLocation);
				ATimberPlayerProjectile* Projectile = GetWorld()->SpawnActorDeferred<ATimberPlayerProjectile>(
					ProjectileType,
					ProjectileSpawnTransform, this);

				Projectile->PlayerProjectileOwner = Cast<ATimberPlayableCharacter>(WeaponOwner);

				Projectile->FinishSpawning(ProjectileSpawnTransform);

				if (Projectile)
				{
					//Setting Timer to next shot
					
					GetWorld()->GetTimerManager().SetTimer(TimeBetweenShotsHandle, this, 
					&ATimberWeaponRangedBase::ResetFiringCooldown, TimeBetweenProjectiles, false);

					//Setting Firing to on Cooldown
					bIsFireOnCooldown = true;

					//Removing Ammo from Available Ammo
					CurrentAmmo--;

					if (CurrentAmmo == 0)
					{
						ATimberPlayerController* PlayerController = Cast<ATimberPlayerController>(Cast<ATimberPlayableCharacter>(GetOwner())->GetController());
						if (PlayerController)
						{
							//Automatic Reload at 0 Ammo.
							PlayerController->ReloadWeapon(true);
						}
					}

					//Playing firing sounds
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), FiringSound, ProjectileSpawnLocation);
					
					Projectile->SetOwner(this);
					
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
				//GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::Green, "Projectile All Variables Loaded");
				FVector ProjectileSpawnLocation = ProjectileSpawnComponent->GetComponentLocation();

				//Get Player Character
				FRotator ControllerDirection = Cast<ATimberCharacterBase>(WeaponOwner)->GetController()->GetControlRotation();
				FRotator RandomAimOffset = FRotator(
					FMath::RandRange(-1, 1), FMath::RandRange(-1, 1), FMath::RandRange(-1, 1));

				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this; //Weapon is owner of the projectile.
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				
				ATimberProjectileBase* Projectile = GetWorld()->SpawnActor<ATimberProjectileBase>(
					ProjectileType, ProjectileSpawnLocation, ControllerDirection + RandomAimOffset);

				if (Projectile)
				{
					//Cooldown for AI Automatic Fire.
					GetWorld()->GetTimerManager().SetTimer(TimeBetweenShotsHandle, this, &ATimberWeaponRangedBase::ResetFiringCooldown, TimeBetweenProjectiles, false);
					bIsFireOnCooldown = true;
					
					UGameplayStatics::PlaySoundAtLocation(this, FiringSound, ProjectileSpawnLocation);
					
					Projectile->SetOwner(this);
					
					//GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::Green, "Projectile Spawned.");
				}
			}
		}
	}
}

void ATimberWeaponRangedBase::PlayReloadMontage()
{
	ATimberPlayableCharacter* PlayerCharacter = Cast<ATimberPlayableCharacter>(WeaponOwner);
	if (PlayerCharacter && PlayerCharacter->ReloadMontage)
	{
		
		//Getting the PLayers Anim Instance so we can bind Delegates
		UAnimInstance* PlayerAnimInstance = PlayerCharacter->GetMesh()->GetAnimInstance();
		if (!PlayerAnimInstance) return;

		//Creating Delegate Call for Early Exit of Reload Montage
		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindUObject(this, &ATimberWeaponRangedBase::HandleReloadMontageInterruption);

		//Setting Delegate on this Exact Montage
		PlayerAnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, PlayerCharacter->ReloadMontage);
		
		//Plays the montage. Notify on Montage end handles the Reload.
		PlayerCharacter->PlayAnimMontage(PlayerCharacter->ReloadMontage, 1.f, FName("Reload1"));

		//Setting the Reload Bool to True - If Animation Notify Runs, this will be reset.
		bIsReloading = true;
	}
}

void ATimberWeaponRangedBase::HandleReloadMontageInterruption(UAnimMontage* Montage, bool bInterrupted)
{
	if (!Montage) return;
	ATimberPlayableCharacter* PlayerCharacter = Cast<ATimberPlayableCharacter>(WeaponOwner);
	UAnimInstance* PlayerAnimInstance = PlayerCharacter->GetMesh()->GetAnimInstance();
	if (PlayerAnimInstance)
	{
		//This is us basically unbinding the delegates from the Montage.
		//We just set it to an empty delegate.
		FOnMontageEnded EmptyDelegate;
		PlayerAnimInstance->Montage_SetEndDelegate(EmptyDelegate, PlayerCharacter->ReloadMontage);
	};

	if (bInterrupted)
	{
		//Even if the Montage Plays past the Notify and gets reset there, and a Montage Interrupt is called, it's ok as long as the Reload Bool is reset to false.
		bIsReloading = false;
	}
	
}

void ATimberWeaponRangedBase::ReloadWeapon()
{
	//Called from Anim Notify on the Anim Montage for the Reload Animation.
	CurrentAmmo = MaxAmmo;
	bIsReloading = false;
}
