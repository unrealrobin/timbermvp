// Property of Paracosm.


#include "Components/Combat/CombatComponent.h"

#include "Character/TimberAnimInstance.h"
#include "Character/TimberPlayableCharacter.h"
#include "UObject/FastReferenceCollector.h"
#include "Weapons/TimberWeaponMeleeBase.h"
#include "Weapons/TimberWeaponRangedBase.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	//Using A Character because a character will always be the owner of this component. Has Mesh.
	OwningCharacter = Cast<ACharacter>(GetOwner());
	UE_LOG(LogTemp, Warning, TEXT("OwningCharacter: %s"), *OwningCharacter->GetName());
	
}

void UCombatComponent::TickComponent(
	float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::SpawnRangedWeapon()
{
	SpawnWeaponAtSocketLocation(RangedWeaponClass,UnEquippedRangeSocket);
}

void UCombatComponent::SpawnMeleeWeapon()
{
	SpawnWeaponAtSocketLocation(MeleeWeaponClass, UnEquippedMeleeSocket);
}

void UCombatComponent::SpawnWeaponAtSocketLocation(TSubclassOf<ATimberWeaponBase> WeaponClassToSpawn, FName SocketName)
{
	// Spawning and Attaching the Weapon to the Socket of Right Hand on Leeroy
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	
	//Ensuring Sockets exists on the Mesh
	if (OwningCharacter && OwningCharacter->GetMesh() && OwningCharacter->GetMesh()->GetSocketByName(SocketName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Socket: %s exists on the Mesh"), *SocketName.ToString());
		FTransform SocketWorldTransform = OwningCharacter->GetMesh()->GetSocketTransform(SocketName, RTS_World);
		FVector SocketWorldLocation = SocketWorldTransform.GetLocation();
		FRotator SocketWorldRotation = SocketWorldTransform.Rotator();
		
		//Spawn the Weapon
		ATimberWeaponBase* SpawnedActor = GetWorld()->SpawnActor<ATimberWeaponBase>(
			WeaponClassToSpawn,
			SocketWorldLocation,
			SocketWorldRotation,
			SpawnParams);
		
		if (SpawnedActor)
		{
			//Not Actually UnEquipping were doing the Initial Socket Attachment to the Unequipped Slot.
			if (ATimberWeaponRangedBase* SpawnedRangedWeapon = Cast<ATimberWeaponRangedBase>(SpawnedActor))
			{
				RangedWeaponInstance = SpawnedRangedWeapon;
				UnEquipWeapon(RangedWeaponInstance, UnEquippedRangeSocket);
			}
			else if (ATimberWeaponMeleeBase* SpawnedMeleeWeapon = Cast<ATimberWeaponMeleeBase>(SpawnedActor))
			{
				MeleeWeaponInstance = SpawnedMeleeWeapon;
				UnEquipWeapon(MeleeWeaponInstance, UnEquippedMeleeSocket);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Socket: %s does not exist on the Mesh"), *SocketName.ToString());
	}
	
}

FVector UCombatComponent::GetProjectileTargetLocation()
{
	/*
	 * Performs Raycast from the camera to the center of the screen and aligns the reticule to the hit location.
	 */
	FVector CameraLocation;
	FVector CameraDirection;

	int ViewportSizeX;
	int ViewportSizeY;

	if (APlayerController* PC = Cast<APlayerController>(OwningCharacter->GetController()))
	{
		PC->GetViewportSize(ViewportSizeX, ViewportSizeY);

		FVector2d ScreenCenter = FVector2d(ViewportSizeX * 0.5f, ViewportSizeY * 0.5f);

		/*
		* The Camera is the screen as you see it.
		* Line Trace goes from the center of the screen (where reticule should be) out to the world by X (10,000.f) units.
		* Expensive, but that's why we only want 1 hit result.
		*/
		if (PC->DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, CameraLocation, CameraDirection))
		{
			if (CameraDirection.Normalize())
			{
				FHitResult HitResult;
				FVector End = CameraLocation + (CameraDirection * 3000.f);
				
				FCollisionQueryParams QueryParams;
				QueryParams.AddIgnoredActor(OwningCharacter);
				QueryParams.AddIgnoredActor(CurrentlyEquippedWeapon);
				bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, End, ECC_Visibility);

				/* Here we handle how to adjust Raycast End for UpClose Targets.*/
				if (bHit)
				{
					//If the Target Object is closer than 500 Units. Only Adjusting for Close Targets.
					if (FVector::Dist(HitResult.ImpactPoint, CameraLocation) < ProjectileAlignmentAdjustmentDistance)
					{
						return HitResult.ImpactPoint;
					}
					return End;
				}
				return End;
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Could not Retrieve Controller from Owner."));
	return FVector::ZeroVector;
}

void UCombatComponent::EquipWeapon(ATimberWeaponBase* WeaponInstance, FName EquippedWeaponSocketName)
{
	if (WeaponInstance)
	{
		WeaponInstance->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, EquippedWeaponSocketName);

		CurrentlyEquippedWeapon = WeaponInstance;

		if (ATimberWeaponRangedBase* RangedWeapon = Cast<ATimberWeaponRangedBase>(CurrentlyEquippedWeapon))
		{
			UpdateCurrentWeaponState(EOwnerWeaponState::RangedWeaponEquipped);

			//Clean up of Reloading State during Ranged Equip.
			RangedWeapon->bIsReloading = false;
			Cast<UTimberAnimInstance>(OwningCharacter->GetMesh()->GetAnimInstance())->bIsReloading = false;
			bIsRifleEquipped = true;
		}
		else if (Cast<ATimberWeaponMeleeBase>(CurrentlyEquippedWeapon))
		{
			UpdateCurrentWeaponState(EOwnerWeaponState::MeleeWeaponEquipped);
			bIsMeleeEquipped = true;
			bCanMeleeAttack = true;
		}
		else
		{
			//Catch all
			UpdateCurrentWeaponState(EOwnerWeaponState::Default);
			UE_LOG(LogTemp, Warning, TEXT("Combat Component - EquipWeapon() - Weapon Instance not of a valid type."));
		}

		
	}
}

void UCombatComponent::UnEquipWeapon(ATimberWeaponBase* WeaponInstance, FName UnEquipSocketName)
{
	//Place the Ranged Weapon on the Owners UnEquipped Ranged Socket Location
	if (WeaponInstance)
	{
		if (WeaponInstance == RangedWeaponInstance)
		{
			bIsRifleEquipped = false;
		}
		else if (WeaponInstance == MeleeWeaponInstance)
		{
			bIsMeleeEquipped = false;
		}

		CurrentlyEquippedWeapon = nullptr;
		
		WeaponInstance->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, 
		UnEquipSocketName);

		UpdateCurrentWeaponState(EOwnerWeaponState::Unequipped);
	}
}

void UCombatComponent::UnEquipCurrentlyEquippedWeapon()
{
	//Default is a catch-all, potentially not even possible to achieve.
	if (CurrentWeaponState == EOwnerWeaponState::Default || CurrentWeaponState == EOwnerWeaponState::Unequipped)
	{
		UE_LOG(LogTemp, Warning, TEXT("Combat Component - UnEquipCurrentlyEquippedWeapon() - No Weapon to UnEquip"));
		return;
	}

	if (CurrentWeaponState == EOwnerWeaponState::RangedWeaponEquipped && RangedWeaponInstance)
	{
		UnEquipWeapon(RangedWeaponInstance, UnEquippedRangeSocket);
	}
	else if (CurrentWeaponState == EOwnerWeaponState::MeleeWeaponEquipped && MeleeWeaponInstance)
	{
		UnEquipWeapon(MeleeWeaponInstance, UnEquippedMeleeSocket);
	}
}

void UCombatComponent::EquipMelee()
{
	if (CurrentlyEquippedWeapon)
	{
		UnEquipCurrentlyEquippedWeapon();
	}
	
	EquipWeapon(MeleeWeaponInstance, EquippedMeleeSocket);
	bIsEquipMontagePlaying = false;
}

void UCombatComponent::EquipRanged()
{
	if (CurrentlyEquippedWeapon)
	{
		UnEquipCurrentlyEquippedWeapon();
	}
	EquipWeapon(RangedWeaponInstance, EquippedRangeSocket);
	bIsEquipMontagePlaying = false;
}

void UCombatComponent::UnEquipAllWeapons()
{
	UnEquipWeapon(MeleeWeaponInstance, UnEquippedMeleeSocket);
	UnEquipWeapon(RangedWeaponInstance, UnEquippedRangeSocket);
}

void UCombatComponent::SendWeaponStateToOwnerAnimInstance()
{
	if (OwningCharacter && OwningCharacter->GetMesh()->GetAnimInstance())
	{
		if (UTimberAnimInstance* Anim = Cast<UTimberAnimInstance>(OwningCharacter->GetMesh()->GetAnimInstance()))
		{
			//Anim Instance will handle what to do with that information.
			Anim->UpdateOwnerWeaponState(CurrentWeaponState);
		}
	}
}

bool UCombatComponent::bHasEnoughPower(float AbilityCost, float CurrentWeaponPower)
{
	bool HasEnoughPower = AbilityCost < CurrentWeaponPower ? true : false;

	return HasEnoughPower;
}

void UCombatComponent::ConsumePower(ATimberWeaponBase* WeaponInstance, float AmountToConsume)
{
	if (WeaponInstance)
	{
		if (bHasEnoughPower(AmountToConsume, WeaponInstance->CurrentPower))
		{
			WeaponInstance->CurrentPower -= AmountToConsume;
			UE_LOG(LogTemp, Warning, TEXT("Consumed: %f, Remaining Power: %f"), AmountToConsume, WeaponInstance->CurrentPower);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Not Enough Power to Consume."));
		}
	}
}

void UCombatComponent::UpdateCurrentWeaponState(EOwnerWeaponState NewWeaponState)
{
	CurrentWeaponState = NewWeaponState;

	// We can do any broadcasts needed or value changes directly from here.
	SendWeaponStateToOwnerAnimInstance();
}

void UCombatComponent::HandlePrimaryAbility()
{
	UE_LOG(LogTemp, Warning, TEXT("Initiating Primary Ability"));
	//Basic LMB Attack for any / all weapons. Called from Player Controller Input Mapping Subsystem
	switch (CurrentWeaponState)
	{
		case EOwnerWeaponState::MeleeWeaponEquipped:
			if (MeleeWeaponInstance && bIsMeleeEquipped && bCanMeleeAttack)
			{
				bCanMeleeAttack = false;
				//Standard attack on the Melee Weapon
				MeleeWeaponInstance->PerformStandardAttack();
			}
			break;
		
		case EOwnerWeaponState::RangedWeaponEquipped:
			if (RangedWeaponInstance && bIsRifleEquipped)
			{
				UE_LOG(LogTemp, Warning, TEXT("Attempting Ranged Primary Ability."));
				//Needs a hit location to fire at.
				//We can perform a Raycast here for 1 Frame to get a Hit Location.
				//RangedWeaponInstance->FireRangedWeapon(GetProjectileTargetLocation());
				//Checking if ability is affordable.
				if (bHasEnoughPower(RangedWeaponInstance->PrimaryAbility.AbilityPowerCost, RangedWeaponInstance->CurrentPower))
				{
					//Initiating the Primary Abilities Execute Logic Flow.
					RangedWeaponInstance->PrimaryAbility.Execute(GenerateCurrentAbilityContext());
					
					//Charging for Ability from Power.
					ConsumePower(RangedWeaponInstance, RangedWeaponInstance->PrimaryAbility.AbilityPowerCost);
				}
			}
			break;
		
		case EOwnerWeaponState::Unequipped:
			break;
		case EOwnerWeaponState::Default:
			break;
	}
	
}

void UCombatComponent::ReloadRangedWeapon()
{
	if (CurrentWeaponState == EOwnerWeaponState::RangedWeaponEquipped && CurrentlyEquippedWeapon == RangedWeaponInstance)
	{
		if (UTimberAnimInstance* Anim = Cast<UTimberAnimInstance>(OwningCharacter->GetMesh()->GetAnimInstance()))
		{
			if (!RangedWeaponInstance->bIsReloading && !Anim->bIsReloading)
			{
				if (RangedWeaponInstance->CurrentAmmo != RangedWeaponInstance->MaxAmmo)
				{
					Anim->bIsReloading = true;
					RangedWeaponInstance->PlayReloadMontage();
				}
			}
		}
		
	}
}

void UCombatComponent::PlayEquipWeaponMontage(FName MontageSectionName)
{
	bIsEquipMontagePlaying = true;

	//Need to call the PlayEquipWeaponMontage on the Player Character but want to avoid Coupling/Casting to that specific Character.
	if (ICombatComponentAnimUser* AnimUser = Cast<ICombatComponentAnimUser>(OwningCharacter))
	{
		AnimUser->PlayWeaponEquipAnimationMontage(MontageSectionName);
	}
}

FAbilityContext UCombatComponent::GenerateCurrentAbilityContext()
{
	FAbilityContext CurrentAbilityContext;
	
	CurrentAbilityContext.Instigator = OwningCharacter;

	CurrentAbilityContext.WeaponInstance = CurrentlyEquippedWeapon;

	CurrentAbilityContext.TargetLocation = GetProjectileTargetLocation();

	return CurrentAbilityContext;
}




