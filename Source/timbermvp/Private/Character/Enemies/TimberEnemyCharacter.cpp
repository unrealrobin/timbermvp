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

void ATimberEnemyCharacter::TakeDamage(float DamageAmount)
{
	CurrentHealth -= DamageAmount;

	if(CurrentHealth <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Target has been Destroyed"));
		Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Target hit for: %f. CurrentHealth: %f."), DamageAmount, CurrentHealth);
	}
}
