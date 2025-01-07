// Property of Paracosm Industries. Dont use my shit.


#include "Character/Enemies/TimberEnemyMeleeBase.h"

#include "Character/TimberPlayableCharacter.h"
#include "Components/CapsuleComponent.h"

ATimberEnemyMeleeBase::ATimberEnemyMeleeBase()
{
	RightHandCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightHandCapsule"));
	LeftHandCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftHandCapsule"));
	RightFootCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightFootCapsule"));
	LeftFootCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftFootCapsule"));

	RightHandCapsuleComponent->SetupAttachment(GetMesh(), FName(TEXT("RHand1 Socket")));
	LeftHandCapsuleComponent->SetupAttachment(GetMesh(), FName(TEXT("LHandCollisionSocket")));
	LeftFootCapsuleComponent->SetupAttachment(GetMesh(), FName(TEXT("LFootCollisionSocket")));
	RightFootCapsuleComponent->SetupAttachment(GetMesh(), FName(TEXT("RFootCollisionSocket")));


	RightHandCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ATimberEnemyMeleeBase::HandleCapsuleOverlap);
	LeftHandCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ATimberEnemyMeleeBase::HandleCapsuleOverlap);
	RightFootCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ATimberEnemyMeleeBase::HandleCapsuleOverlap);
	LeftFootCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ATimberEnemyMeleeBase::HandleCapsuleOverlap);
}

void ATimberEnemyMeleeBase::BeginPlay()
{
	Super::BeginPlay();
}

void ATimberEnemyMeleeBase::EnableCapsuleComponent(UCapsuleComponent* MeleeCapsuleComponent)
{
	MeleeCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeleeCapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeleeCapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	MeleeCapsuleComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
}

void ATimberEnemyMeleeBase::DisableCapsuleComponent(UCapsuleComponent* MeleeCapsuleComponent)
{
	MeleeCapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	MeleeCapsuleComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
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
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::Red, "Capsule Overlap");
		}
		//PlayerCharacter->PlayerTakeDamage(CalculateOutputDamage(BaseMeleeAttackDamage));
		PlayerCharacter->PlayerTakeDamage(BaseMeleeAttackDamage);
	}
}
