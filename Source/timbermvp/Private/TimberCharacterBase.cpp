// Property of Paracosm Industries. Dont use my shit.


#include "TimberCharacterBase.h"

// Sets default values
ATimberCharacterBase::ATimberCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATimberCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATimberCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATimberCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

