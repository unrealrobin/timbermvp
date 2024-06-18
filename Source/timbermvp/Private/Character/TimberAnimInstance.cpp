// Property of Paracosm Industries. Dont use my shit.


#include "Character/TimberAnimInstance.h"

#include "Character/TimberPlayableCharacter.h"
#include "Controller/TimberPlayerController.h"

void UTimberAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	if(!OwningPawn)
	{
		OwningPawn = Cast<ATimberPlayableCharacter>(GetOwningActor());
	}

	if(OwningPawn)
	{
		PlayerController = Cast<ATimberPlayerController>(OwningPawn->GetController());
	}
	
}

void UTimberAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	InitializeReferences();
}

void UTimberAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	
}

void UTimberAnimInstance::InitializeReferences()
{
	//Owner of the Anim Instance
	/*OwningPawn = TryGetPawnOwner();
	if(OwningPawn)
	{
		//Casted to TPC
		Character = Cast<ATimberPlayableCharacter>(OwningPawn);
		PlayerController = Cast<ATimberPlayerController>(OwningPawn->GetController());
		if(PlayerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Controller found: %s"), *PlayerController->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Controller not available."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Owning Pawn not available."));
	}*/
}
