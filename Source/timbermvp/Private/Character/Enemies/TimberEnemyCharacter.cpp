// Property of Paracosm Industries. Dont use my shit.


#include "Character/Enemies/TimberEnemyCharacter.h"

#include "Components/CapsuleComponent.h"

ATimberEnemyCharacter::ATimberEnemyCharacter()
{
}

void ATimberEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	// Ignoring all channels temporarily for testing.
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	// Setting the WorldStatic Channel to Overlap. Axe weapon is World Static and we want to generate overlap events.
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap); // WIll OVERLAP AXE
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
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
