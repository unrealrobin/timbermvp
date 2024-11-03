// Property of Paracosm Industries. Dont use my shit.


#include "Character/Enemies/Boss/BossBruiser.h"


// Sets default values
ABossBruiser::ABossBruiser()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MaxHealth = 500.0f;
	CurrentHealth = MaxHealth;
}

// Called when the game starts or when spawned
void ABossBruiser::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossBruiser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABossBruiser::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

