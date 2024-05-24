// Property of Paracosm Industries. Dont use my shit.


#include "Character/Enemies/TimberEnemyCharacter.h"

#include "Character/TimberPlayableCharacter.h"
#include "Components/CapsuleComponent.h"

ATimberEnemyCharacter::ATimberEnemyCharacter()
{
	KickCollisionSphere = CreateDefaultSubobject<UCapsuleComponent>("KickCollisionSphere");
	KickCollisionSphere->SetupAttachment(GetMesh(), "KickCollisionSocket");

	KickCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ATimberEnemyCharacter::HandleKickOverlap);
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
		
		Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Target hit for: %f. CurrentHealth: %f."), DamageAmount, CurrentHealth);
	}
}

void ATimberEnemyCharacter::HandleKickOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//TODO::Handle the kick colliding with the Player and applying damage.

	ATimberPlayableCharacter* Player = Cast<ATimberPlayableCharacter>(OtherActor);

	if(Player)
	{
		if(Player->CurrentHealth > 0.f)
		{
			Player->CurrentHealth -= StandardMelleAttackDamage;
		}
		else
		{
			if(GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player is Dead"));
			}
		}
		
	}
	
}

void ATimberEnemyCharacter::DisableKickCollision()
{
	KickCollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATimberEnemyCharacter::EnableKickCollision()
{
	KickCollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	KickCollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	KickCollisionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}
