// Property of Paracosm Industries. Dont use my shit.


#include "Character/TimberAnimInstance.h"

#include "Character/TimberPlayableCharacter.h"
#include "Controller/TimberPlayerController.h"

void UTimberAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	if(!TimberPlayerController)
	{
		TimberPlayerController= Cast<ATimberPlayerController>(TimberPlayableCharacter->GetController());
	}
}

void UTimberAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwningActor = GetOwningActor();
	if(!OwningActor)
	{
		return;
	}
	TimberPlayableCharacter = Cast<ATimberPlayableCharacter>(OwningActor);
	
	
	
}

void UTimberAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if(TimberPlayerController && TimberPlayableCharacter)
	{
		InputActionValue = TimberPlayerController->GetMoveInputActionValue();
	}
	
}
