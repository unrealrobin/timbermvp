// Property of Paracosm Industries. Don't use my shit.


#include "Character/Enemies/Boss/BossLola.h"
#include "Components/CapsuleComponent.h"


// Sets default values
ABossLola::ABossLola()
{
	
	PrimaryActorTick.bCanEverTick = true;
	LolaInitializeComponents();
}


void ABossLola::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABossLola::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABossLola::LolaInitializeComponents()
{
	HeadCollisionComponent = CreateDefaultSubobject<UCapsuleComponent>("HeadCollisionComponent");
	LeftArmCollisionComponent = CreateDefaultSubobject<UCapsuleComponent>("LeftArmCollisionComponent");
	LeftArmCollisionComponent->SetupAttachment(RootComponent);
	RightArmCollisionComponent = CreateDefaultSubobject<UCapsuleComponent>("RightArmCollisionComponent");
	RightArmCollisionComponent->SetupAttachment(RootComponent);

	/* Minions */
	Drone1 = CreateDefaultSubobject<UChildActorComponent>("Drone1");
	Drone2 = CreateDefaultSubobject<UChildActorComponent>("Drone2");
	Drone3 = CreateDefaultSubobject<UChildActorComponent>("Drone3");
	Drone1->SetupAttachment(RootComponent);
	Drone2->SetupAttachment(RootComponent);
	Drone3->SetupAttachment(RootComponent);


	
	
	
}

