// Property of Paracosm Industries. Dont use my shit.


#include "BuildSystem/BuildableBase.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
ABuildableBase::ABuildableBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABuildableBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = GameplayTagContainer;
}

// Called when the game starts or when spawned
void ABuildableBase::BeginPlay()
{
	Super::BeginPlay();
}

void ABuildableBase::HandleDeletionOfBuildable()
{
	//TODO:: Play Deletion Sound
	SpawnLootInRange(BuildableCost.CostOfParts, BuildableCost.CostOfMechanisms, BuildableCost.CostOfUniques);
	Destroy();
}

void ABuildableBase::SpawnLootInRange(int NumberOfParts, int NumberOfMechanisms, int NumberOfUniques)
{
	FActorSpawnParameters SpawnParameters;
	FVector ActorLocation = GetActorLocation();
	ActorLocation.Z += 100.0f;
	for(int i = 0; i < NumberOfParts; i++)
	{
		FVector SpawnLocation;
		//Random Location around the Broken Building Component
		SpawnLocation = FVector(FMath::RandRange(-50, 50) + ActorLocation.X, FMath::RandRange(-50, 50) + ActorLocation.Y, FMath::RandRange(0, 20) + ActorLocation.Z );
		//Spawn Parts in Random Range of Actors Location
		GetWorld()->SpawnActor<AEnemyLootDropBase>(PartsClass, SpawnLocation, FRotator::ZeroRotator, SpawnParameters);
		
	}

	for(int i = 0; i < NumberOfMechanisms; i++)
	{
		FVector SpawnLocation;
		//Random Location around the Broken Building Component
		SpawnLocation = FVector(FMath::RandRange(-50, 50) + ActorLocation.X, FMath::RandRange(-50, 50) + ActorLocation.Y, FMath::RandRange(0, 20) + ActorLocation.Z );
		//Spawn Parts in Random Range of Actors Location
		GetWorld()->SpawnActor<AEnemyLootDropBase>(MechanismsClass, SpawnLocation, FRotator::ZeroRotator, 
		SpawnParameters);
		
	}

	for(int i = 0; i < NumberOfUniques; i++)
	{
		FVector SpawnLocation;
		//Random Location around the Broken Building Component
		SpawnLocation = FVector(FMath::RandRange(-50, 50) + ActorLocation.X, FMath::RandRange(-50, 50) + ActorLocation.Y, FMath::RandRange(0, 20) + ActorLocation.Z );
		//Spawn Parts in Random Range of Actors Location
		GetWorld()->SpawnActor<AEnemyLootDropBase>(UniquesClass, SpawnLocation, FRotator::ZeroRotator, SpawnParameters);
		
	}
	
}

// Called every frame
void ABuildableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
