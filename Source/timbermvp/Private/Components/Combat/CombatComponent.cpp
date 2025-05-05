// Property of Paracosm.


#include "Components/Combat/CombatComponent.h"

#include "Character/TimberAnimInstance.h"
#include "Character/TimberPlayableCharacter.h"
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

void UCombatComponent::EquipWeapon(ATimberWeaponBase* WeaponInstance, FName EquippedWeaponSocketName)
{
	if (WeaponInstance)
	{
		WeaponInstance->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, EquippedWeaponSocketName);

		CurrentlyEquippedWeapon = WeaponInstance;

		if (Cast<ATimberWeaponRangedBase>(CurrentlyEquippedWeapon))
		{
			UpdateCurrentWeaponState(EOwnerWeaponState::RangedWeaponEquipped);
		}
		else if (Cast<ATimberWeaponMeleeBase>(CurrentlyEquippedWeapon))
		{
			UpdateCurrentWeaponState(EOwnerWeaponState::MeleeWeaponEquipped);
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
		WeaponInstance->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, 
		UnEquipSocketName);

		UpdateCurrentWeaponState(EOwnerWeaponState::Unequipped);
	}
}

void UCombatComponent::EquipMelee()
{
	if (CurrentlyEquippedWeapon)
	{
		UnEquipAllWeapons();
	}
	
	EquipWeapon(MeleeWeaponInstance, EquippedMeleeSocket);
}

void UCombatComponent::EquipRanged()
{
	if (CurrentlyEquippedWeapon)
	{
		UnEquipAllWeapons();
	}
	EquipWeapon(RangedWeaponInstance, EquippedRangeSocket);
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

void UCombatComponent::UnEquipAllWeapons()
{
	UnEquipWeapon(MeleeWeaponInstance, UnEquippedMeleeSocket);
	UnEquipWeapon(RangedWeaponInstance, UnEquippedRangeSocket);
}

void UCombatComponent::UpdateCurrentWeaponState(EOwnerWeaponState NewWeaponState)
{
	CurrentWeaponState = NewWeaponState;

	// We can do any broadcasts needed or value changes directly from here.
	SendWeaponStateToOwnerAnimInstance();
}

