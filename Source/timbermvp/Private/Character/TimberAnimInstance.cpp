// Property of Paracosm Industries.


#include "Character/TimberAnimInstance.h"
#include "Character/TimberPlayableCharacter.h"
#include "Controller/TimberPlayerController.h"

void UTimberAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	if (!OwningPawn)
	{
		OwningPawn = Cast<ATimberPlayableCharacter>(GetOwningActor());
		OwningPawn = Cast<ATimberPlayableCharacter>(TryGetPawnOwner());
	}

	if (OwningPawn)
	{
		PlayerController = Cast<ATimberPlayerController>(OwningPawn->GetController());
	}
}

void UTimberAnimInstance::UpdateOwnerWeaponState(EOwnerWeaponState OwnerWeaponState)
{
	AnimCurrentWeaponState = OwnerWeaponState;
	
	if (AnimCurrentWeaponState == EOwnerWeaponState::RangedWeaponEquipped)
	{
		bIsRangedWeaponEquipped = true;
	}
	else
	{
		bIsRangedWeaponEquipped = false;
	}
}

