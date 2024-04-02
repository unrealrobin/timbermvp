// Property of Paracosm Industries. Dont use my shit.


#include "Character/TimberCharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/MovementComponent.h"

// Sets default values
ATimberCharacterBase::ATimberCharacterBase()
{
 	PrimaryActorTick.bCanEverTick = false;
	
	

}

// Called when the game starts or when spawned
void ATimberCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATimberCharacterBase::DecreaseHealth(int DecreaseAmount)
{
	if(CurrentHealth - DecreaseAmount <= 0 )
	{
		Destroy();
	}

	CurrentHealth -= DecreaseAmount;
	
}





