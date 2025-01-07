// Property of Paracosm Industries. Dont use my shit.


#include "Character/TimberAnimInstance.h"

#include "Character/TimberPlayableCharacter.h"
#include "Controller/TimberPlayerController.h"

void UTimberAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	if (!OwningPawn)
	{
		OwningPawn = Cast<ATimberPlayableCharacter>(GetOwningActor());
	}

	if (OwningPawn)
	{
		PlayerController = Cast<ATimberPlayerController>(OwningPawn->GetController());
	}
}

void UTimberAnimInstance::InitializeDelegates()
{
}

void UTimberAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UTimberAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
}

void UTimberAnimInstance::OnWeaponStateChange(EWeaponState WeaponState)
{
	switch (WeaponState)
	{
	case EWeaponState::RangedEquipped:
		isRangedEquipped = true;
		break;
	case EWeaponState::AxeEquipped:
		isRangedEquipped = false;
		break;
	case EWeaponState::ChainsawEquipped:
		isRangedEquipped = false;
		break;
	case EWeaponState::Unequipped:
		isRangedEquipped = false;
		break;
	default:
		isRangedEquipped = false;
		break;
	}
}
