﻿// Property of Paracosm Industries. Dont use my shit.


#include "Weapons/TimberWeaponRangedBase.h"
#include "Character/TimberCharacterBase.h"
#include "Controller/TimberPlayerController.h"
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

void ATimberWeaponRangedBase::FireRangedWeapon()
{
	GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::Green, "Firing Ranged Weapon.");
	
	if(WeaponOwner && GetWorld())
	{
		if(ProjectileType)
		{
			GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::Green, "Projectile All Variables Loaded");
			FVector ProjectileSpawnLocation = ProjectileSpawnComponent->GetComponentLocation();
			FRotator ControllerDirection = Cast<ATimberCharacterBase>(WeaponOwner)->GetController()->GetControlRotation();

			ATimberProjectileBase* Projectile = GetWorld()->SpawnActor<ATimberProjectileBase>(ProjectileType, ProjectileSpawnLocation, ControllerDirection);

			if(Projectile)
			{
				Projectile->SetOwner(this);
				GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::Green, "Projectile Spawned.");
			}
		}
	}
	
	
}

void ATimberWeaponRangedBase::PerformStandardAttack()
{
}
