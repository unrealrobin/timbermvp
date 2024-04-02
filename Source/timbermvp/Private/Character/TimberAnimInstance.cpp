// Property of Paracosm Industries. Dont use my shit.


#include "Character/TimberAnimInstance.h"

#include "Character/TimberCharacterBase.h"
#include "Controller/TimberPlayerController.h"

void UTimberAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	
	
	
}

void UTimberAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	// Setting InputActionValue Every Tick to track Controller Input for Character Movement
	AActor* OwningActor = GetOwningActor();
	if(!OwningActor) return;
	TimberCharacterBase = Cast<ATimberCharacterBase>(OwningActor);
	if(!TimberCharacterBase) return;
	TimberPlayerController= Cast<ATimberPlayerController>(TimberCharacterBase->GetController());
	
	if(TimberPlayerController && TimberCharacterBase)
	{
		InputActionValue = TimberPlayerController->GetInputActionValue();
	}
	
	
}
