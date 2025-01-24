// Property of Paracosm Industries. Dont use my shit.


#include "Character/TimberSeeda.h"

#include "Components/CapsuleComponent.h"


// Sets default values
ATimberSeeda::ATimberSeeda()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	CollisionSphere = CreateDefaultSubobject<UCapsuleComponent>("Collision Sphere");
	RootComponent = CollisionSphere;
	StaticMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATimberSeeda::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATimberSeeda::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATimberSeeda::TakeDamage_Seeda(float DamageAmount)
{
	CurrentHealth -= DamageAmount;
	UE_LOG(LogTemp, Warning, TEXT("Seeda Took %f Damage. Seeda Health: %f"), DamageAmount, CurrentHealth);
	if (CurrentHealth <= 0)
	{
		OnSeedaDeath.Broadcast(true);
		Destroy();
	}
	
}
