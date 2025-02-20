// Property of Paracosm Industries. Dont use my shit.


#include "Loot/LootHealthDrop.h"

#include "Character/TimberPlayableCharacter.h"
#include "Components/CapsuleComponent.h"


// Sets default values
ALootHealthDrop::ALootHealthDrop()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALootHealthDrop::BeginPlay()
{
	Super::BeginPlay();

	CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &ALootHealthDrop::HandlePlayerOverlap);

	//Health Drops are available for 20 seconds
	GetWorld()->GetTimerManager().SetTimer(HealthAvailableTimerHandle, this, &ALootHealthDrop::HandleDestroy, 
	HealthAvailableTime, 
	false );
}

// Called every frame
void ALootHealthDrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ALootHealthDrop::HandlePlayerOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	ATimberPlayableCharacter* PlayerCharacter = Cast<ATimberPlayableCharacter>(OtherActor);
	
	if (PlayerCharacter)
	{
		float MaxHealth = PlayerCharacter->MaxHealth;
		float CurrentHealth = PlayerCharacter->CurrentHealth;

		// Avoid Pickup if at Max Health
		if (MaxHealth == CurrentHealth) return;

		//If the health gain amount is greater than the max health, set the player's health to max health - No Overcapping
		if (CurrentHealth + HealthGainAmount > MaxHealth)
		{
			PlayerCharacter->CurrentHealth = MaxHealth;
			HandleDestroy();
		}
		else
		{
			PlayerCharacter->CurrentHealth += HealthGainAmount;
			HandleDestroy();
		}
			PlaySFX();
	}
}

void ALootHealthDrop::HandleDestroy()
{
	Destroy();
}


