// Property of Paracosm Industries. Dont use my shit.


#include "Character/Enemies/TimberEnemyMeleeBase.h"

#include "BuildSystem/BuildableBase.h"
#include "BuildSystem/BuildingComponents/TimberBuildingComponentBase.h"
#include "Character/TimberPlayableCharacter.h"
#include "Character/TimberSeeda.h"
#include "Components/CapsuleComponent.h"

ATimberEnemyMeleeBase::ATimberEnemyMeleeBase()
{
	RightHandCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightHandCapsule"));
	LeftHandCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftHandCapsule"));
	RightFootCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightFootCapsule"));
	LeftFootCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftFootCapsule"));

	RightHandCapsuleComponent->SetupAttachment(GetMesh(), FName(TEXT("RHandCollisionSocket")));
	LeftHandCapsuleComponent->SetupAttachment(GetMesh(), FName(TEXT("LHandCollisionSocket")));
	LeftFootCapsuleComponent->SetupAttachment(GetMesh(), FName(TEXT("LFootCollisionSocket")));
	RightFootCapsuleComponent->SetupAttachment(GetMesh(), FName(TEXT("RFootCollisionSocket")));


	RightHandCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ATimberEnemyMeleeBase::HandleCapsuleOverlap);
	LeftHandCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ATimberEnemyMeleeBase::HandleCapsuleOverlap);
	RightFootCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ATimberEnemyMeleeBase::HandleCapsuleOverlap);
	LeftFootCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ATimberEnemyMeleeBase::HandleCapsuleOverlap);

	EnemyType = EEnemyType::BasicRobot;
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

	//Empties the array of actors to ignore after the completion of the hand and leg swing.
	ActorsToIgnore.Empty();
}

float ATimberEnemyMeleeBase::CalculateOutputDamage(float Damage)
{
	return Super::CalculateOutputDamage(Damage);
}

void ATimberEnemyMeleeBase::HandleCapsuleOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	
	
	if(ActorsToIgnore.Contains(OtherActor))
	{
		return;
	}
	ATimberPlayableCharacter* PlayerCharacter = Cast<ATimberPlayableCharacter>(OtherActor);
	if (PlayerCharacter && PlayerCharacter->CurrentHealth > 0)
	{
		
		PlayerCharacter->PlayerTakeDamage(BaseMeleeAttackDamage);
		ActorsToIgnore.Add(OtherActor);
	}

	ATimberSeeda* Seeda = Cast<ATimberSeeda>(OtherActor);
	if(Seeda && Seeda->CurrentHealth > 0)
	{
		Seeda->TakeDamage_Seeda(BaseMeleeAttackDamage);
		ActorsToIgnore.Add(OtherActor);
	}

	ATimberBuildingComponentBase* Building = Cast<ATimberBuildingComponentBase>(OtherActor);
	if(Building)
	{
		UE_LOG(LogTemp, Warning, TEXT("Timber Enemy Melee Base Capsule Overlap. Other Actor: %s"), *OtherActor->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Overlapped Component: %s"), *OtherComp->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Melee Robot hit a Building Component and dealt damage"));
		Building->BuildingComponentTakeDamage(BaseMeleeAttackDamage, this);
		ActorsToIgnore.Add(OtherActor);
	}
}
