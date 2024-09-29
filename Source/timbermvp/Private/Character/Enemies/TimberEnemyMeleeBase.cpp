// Property of Paracosm Industries. Dont use my shit.


#include "Character/Enemies/TimberEnemyMeleeBase.h"

#include "Character/TimberPlayableCharacter.h"
#include "Components/CapsuleComponent.h"

ATimberEnemyMeleeBase::ATimberEnemyMeleeBase()
{
	
}

void ATimberEnemyMeleeBase::BeginPlay()
{
	Super::BeginPlay();
}

void ATimberEnemyMeleeBase::EnableCapsuleComponent(UCapsuleComponent* MeleeCapsuleComponent)
{
	MeleeCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeleeCapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	MeleeCapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	//This is the channel for the Building Component
	MeleeCapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);
}

void ATimberEnemyMeleeBase::DisableCapsuleComponent(UCapsuleComponent* MeleeCapsuleComponent)
{
	MeleeCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

float ATimberEnemyMeleeBase::CalculateOutputDamage(float Damage)
{
	return Super::CalculateOutputDamage(Damage);
}

void ATimberEnemyMeleeBase::HandleCapsuleOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	ATimberPlayableCharacter* PlayerCharacter = Cast<ATimberPlayableCharacter>(OtherActor);
	if (PlayerCharacter && PlayerCharacter->CurrentHealth > 0)
	{
		PlayerCharacter->PlayerTakeDamage(CalculateOutputDamage(BaseMeleeAttackDamage));
	}
}
