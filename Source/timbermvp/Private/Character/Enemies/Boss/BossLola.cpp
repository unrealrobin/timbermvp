// Property of Paracosm Industries. Don't use my shit.


#include "Character/Enemies/Boss/BossLola.h"

#include "Character/Enemies/FloaterDrones.h"
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

	//Initializing Drones
	PopulateDronesArray();

	//When a drone dies Lola Get Stunned & isDamageable
	if (Drone1 && Drone2 && Drone3)
	{
		Drone1->DroneDestroyedHandle.AddDynamic(this, &ABossLola::SetLolaToStunned);
		Drone2->DroneDestroyedHandle.AddDynamic(this, &ABossLola::SetLolaToStunned);
		Drone3->DroneDestroyedHandle.AddDynamic(this, &ABossLola::SetLolaToStunned);
	}

	//Initial Drone Vulnerability Randomization
	RandomizeDroneVulnerability();
	//Timer for randomizing Drone Vulnerability
	GetWorld()->GetTimerManager().SetTimer(RandomizeDroneVulnerability_Handle, this, 
	&ABossLola::RandomizeDroneVulnerability, RandomizationTime, true );
}

void ABossLola::PopulateDronesArray()
{
	//Get Actors from ChildActorComponents
	Drone1 = Cast<AFloaterDrones>(Drone1Component->GetChildActor());
	Drone2  = Cast<AFloaterDrones>(Drone2Component->GetChildActor());
	Drone3 = Cast<AFloaterDrones>(Drone3Component->GetChildActor());

	if(Drone1 && Drone2 && Drone3)
	{
		FloaterDronesArray.Add(Drone1);
		FloaterDronesArray.Add(Drone2);
		FloaterDronesArray.Add(Drone3);
	}
}

void ABossLola::RandomizeDroneVulnerability()
{

	//Changing drones vulnerability randomly-ish

	//Dynamic Range
	int RandomNum = FMath::RandRange(0,FloaterDronesArray.Num() - 1);

	for (int i = 0; i < FloaterDronesArray.Num(); i++)
	{
		if (i == RandomNum)
		{
			FloaterDronesArray[i]->HandleChangeDamageState(EDroneState::Damageable);
		}
		else
		{
			FloaterDronesArray[i]->HandleChangeDamageState(EDroneState::NotDamageable);
		}
	}
}

void ABossLola::RemoveDroneFromArray(AFloaterDrones* Drone)
{
	if (FloaterDronesArray.Contains(Drone))
	{
		FloaterDronesArray.Remove(Drone);
	}
}

void ABossLola::SetDronesToNotDamageableDuringStun()
{
	for (int i = 0; i < FloaterDronesArray.Num(); i++)
	{
		FloaterDronesArray[i]->HandleChangeDamageState(EDroneState::NotDamageable);

		//Also Resetting Health
		FloaterDronesArray[i]->CurrentHealth = FloaterDronesArray[i]->MaxHealth;
	}
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
	Drone1Component = CreateDefaultSubobject<UChildActorComponent>("Drone1");
	Drone2Component = CreateDefaultSubobject<UChildActorComponent>("Drone2");
	Drone3Component = CreateDefaultSubobject<UChildActorComponent>("Drone3");
	Drone1Component->SetupAttachment(RootComponent);
	Drone2Component->SetupAttachment(RootComponent);
	Drone3Component->SetupAttachment(RootComponent);
	
}

void ABossLola::SetLolaToStunned(AFloaterDrones* Drone)
{
	LolaState = ELolaState::Stunned;
	
	RemoveDroneFromArray(Drone);
	SetDronesToNotDamageableDuringStun();

	//Pause the Timer for Randomizing Drone Vulnerability
	GetWorld()->GetTimerManager().PauseTimer(RandomizeDroneVulnerability_Handle);

	//Set Timer for Lola to be un-stunned
	GetWorld()->GetTimerManager().SetTimer(LolaStunnedTimer_Handle, this, &ABossLola::SetLolaToNotStunned, LolaStunTime, 
	false);
}


void ABossLola::SetLolaToNotStunned()
{
	LolaState = ELolaState::NotStunned;

	//Randomize Drone Vulnerability - Forced
	RandomizeDroneVulnerability();

	//Unpause the Timer for Randomizing Drone Vulnerability
	GetWorld()->GetTimerManager().UnPauseTimer(RandomizeDroneVulnerability_Handle);
}

ELolaState ABossLola::GetLolaState() const
{
	return LolaState;
}


