// Property of Paracosm Industries. Dont use my shit.


#include "Character/Enemies/TimberEnemyCharacter.h"

ATimberEnemyCharacter::ATimberEnemyCharacter()
{
}

void ATimberEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATimberEnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ATimberEnemyCharacter::TakeDamage()
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 3.0, FColor::Green, "Enemy just got Hit.");
		Destroy();
	}
}
