// Property of Paracosm Industries. Dont use my shit.


#include "Character/Enemies/TimberEnemyCharacter.h"

#include "Character/TimberPlayableCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameModes/TimberGameModeBase.h"

ATimberEnemyCharacter::ATimberEnemyCharacter()
{
	KickCollisionSphere = CreateDefaultSubobject<UCapsuleComponent>("KickCollisionSphere");
	KickCollisionSphere->SetupAttachment(GetMesh(), "KickCollisionSocket");
	KickCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ATimberEnemyCharacter::HandleKickOverlap);
}

void ATimberEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	/* Listening to Delegate Broadcast from TimberGameMode */
	ATimberGameModeBase* GameMode = Cast<ATimberGameModeBase>(GetWorld()->GetAuthGameMode());
	GameMode->CurrentWaveNumberHandle.AddDynamic(this, &ATimberEnemyCharacter::UpdateCurrentWaveNumber);
	
}

void ATimberEnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ATimberEnemyCharacter::TakeDamage(float DamageAmount)
{
	CurrentHealth -= DamageAmount;

	//Used for AI Damage/Aggro System
	//If the player has dealt more than 20 damage to the enemy, the enemy will aggro the player. Causing the BB Value to Change
	if(MaxHealth - CurrentHealth > 20.f)
	{
		bHasBeenAggroByPlayer = true;
	}

	if(CurrentHealth <= 0.f)
	{
		//Checking if the enemy was part of the wave spawn system and thus needs to be tracked.
		ATimberGameModeBase* GameMode = Cast<ATimberGameModeBase>(GetWorld()->GetAuthGameMode());
		GameMode->CheckArrayForEnemy(this);
		Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Target hit for: %f. CurrentHealth: %f."), DamageAmount, CurrentHealth);
	}
}

void ATimberEnemyCharacter::HandleKickOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

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

void ATimberEnemyCharacter::UpdateCurrentWaveNumber(float CurrentWaveNumber)
{
	CurrentWave = CurrentWaveNumber;
}
