// Property of Paracosm Industries. Dont use my shit.


#include "Character/TimberSeeda.h"

#include "Components/CapsuleComponent.h"


// Sets default values
ATimberSeeda::ATimberSeeda()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	StaticMeshComponent->SetupAttachment(RootComponent);
	CollisionSphere = CreateDefaultSubobject<UCapsuleComponent>("Collision Sphere");
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

// Called to bind functionality to input
void ATimberSeeda::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
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
