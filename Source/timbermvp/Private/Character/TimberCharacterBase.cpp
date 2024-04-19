// Property of Paracosm Industries. Dont use my shit.


#include "Character/TimberCharacterBase.h"

#include "Components/CapsuleComponent.h"

// Sets default values
ATimberCharacterBase::ATimberCharacterBase()
{
 	PrimaryActorTick.bCanEverTick = false;
	
}

// Called when the game starts or when spawned
void ATimberCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	UCapsuleComponent* EnemyCapsuleComponent = GetCapsuleComponent();
	
	
}








