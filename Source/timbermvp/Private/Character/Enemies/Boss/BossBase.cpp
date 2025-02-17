// Property of Paracosm Industries. Dont use my shit.


#include "Character/Enemies/Boss/BossBase.h"

#include "Loot/EnemyLootDropBase.h"
#include "Loot/LootHealthDropMax.h"


// Sets default values
ABossBase::ABossBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABossBase::BeginPlay()
{
	Super::BeginPlay();

	OnDestroyed.AddDynamic(this, &ABossBase::BroadcastDeathDelegate);
}

// Called every frame
void ABossBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABossBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABossBase::HandleDropHealthLoot(TSubclassOf<AEnemyLootDropBase> HealthDropClass)
{

	//Bosses always Drop a Max Health
	if (HealthDropClass->IsChildOf(ALootHealthDropMax::StaticClass()))
	{
		SpawnLoot(HealthDropClass);
	}

	//This calls the Original function which has a 5% chance for another Max Health and a 20% chance for a normal health drop.
	Super::HandleDropHealthLoot(HealthDropClass);
}

void ABossBase::BroadcastDeathDelegate(AActor* DestroyedActor)
{
	OnBossDeath.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("BossBase - Broadcasting Boss Death."));
}


