﻿// Property of Paracosm Industries. Dont use my shit.


#include "Character/Enemies/TimberEnemyMeleeWeaponBase.h"

#include "Weapons/TimberWeaponBase.h"


// Sets default values
ATimberEnemyMeleeWeaponBase::ATimberEnemyMeleeWeaponBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATimberEnemyMeleeWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	EquipMeleeWeapon(MeleeWeaponClassName);
	
}

void ATimberEnemyMeleeWeaponBase::EquipMeleeWeapon(TSubclassOf<ATimberWeaponBase> WeaponClassName)
{
	//TODO:: Spawn Weapon in Hand Slot
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = GetInstigator();

	if(GetMesh())
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::Black, "Mesh Found for Melee Weapon Spawn.");
		
		const FTransform RHandSocketTransform = GetMesh()->GetSocketTransform("RHandWeaponSocket");
		if(RHandSocketTransform.IsValid())
		{
			GEngine->AddOnScreenDebugMessage(2, 5.0, FColor::Black, "Right Hand Socket Transform Found");
			AActor* WeaponActor = GetWorld()->SpawnActor<ATimberWeaponBase>(WeaponClassName,
			RHandSocketTransform.GetLocation(), 
			RHandSocketTransform
			.GetRotation().Rotator(),
			SpawnParameters);
	
			EquippedWeapon = Cast<ATimberWeaponBase>(WeaponActor);
			if(EquippedWeapon)
			{
				GEngine->AddOnScreenDebugMessage(3, 5.0, FColor::Black, "Equipped Weapon Casted Correctly");
				EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "RHandWeaponSocket" );
			}
		}
	}
	
}

// Called every frame
void ATimberEnemyMeleeWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
