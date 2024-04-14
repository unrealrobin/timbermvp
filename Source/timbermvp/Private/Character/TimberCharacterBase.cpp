// Property of Paracosm Industries. Dont use my shit.


#include "Character/TimberCharacterBase.h"

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

void ATimberCharacterBase::DecreaseHealth(float DecreaseAmount)
{
	CurrentHealth -= DecreaseAmount;

	if (CurrentHealth <= 0)
	{
		//Kills the Actor and Removes from Level.

		//TODO:: Implement a Destroy Override for Each Enemy to Include a Death Animation.
		Destroy();
	}
}







