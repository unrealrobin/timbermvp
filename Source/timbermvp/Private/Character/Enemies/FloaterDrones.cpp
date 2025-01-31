// Property of Paracosm Industries. Dont use my shit.


#include "Character/Enemies/FloaterDrones.h"

#include "Components/CapsuleComponent.h"


// Sets default values
AFloaterDrones::AFloaterDrones()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AFloaterDrones::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFloaterDrones::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFloaterDrones::TakeDamage(float DamageAmount, AActor* DamageInstigator)
{
	if (DroneState == EDroneState::Damageable)
	{
		//Take Damage
		CurrentHealth = CurrentHealth - DamageAmount;
		if (CurrentHealth <= 0)
		{
			HandleDroneDeath();
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 3.0, FColor::Red, "Drone is not Damageable.");
		}
	}
}

void AFloaterDrones::HandleDroneDeath()
{

	//Broadcast to Lola to change her state to Stunned
	DroneDestroyedHandle.Broadcast(this);
	Destroy();
}

void AFloaterDrones::HandleBaseMaterialColorChange(EDroneState State)
{
	//Changing BaseMaterialColor to Some Color.
	if (State == EDroneState::Damageable)
	{
		GetMesh()->SetMaterial(0, DamageableMaterial);
	}
	else
	{
		GetMesh()->SetMaterial(0, NotDamageableMaterial);
	}
}

void AFloaterDrones::SetActorUnDamageable()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AFloaterDrones::SetActorDamageable()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}



void AFloaterDrones::HandleChangeDamageState(EDroneState NewState)
{
	DroneState = NewState;

	HandleBaseMaterialColorChange(DroneState);

	//Change Collision Parameters Based on current state.
}


