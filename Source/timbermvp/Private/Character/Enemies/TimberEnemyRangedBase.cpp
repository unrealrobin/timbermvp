// Property of Paracosm Industries. Dont use my shit.


#include "Character/Enemies/TimberEnemyRangedBase.h"


// Sets default values
ATimberEnemyRangedBase::ATimberEnemyRangedBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATimberEnemyRangedBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATimberEnemyRangedBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATimberEnemyRangedBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

