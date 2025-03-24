// Property of Paracosm Industries. Dont use my shit.


#include "BuildSystem/Traps/FrostTrap.h"

#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"


// Sets default values
AFrostTrap::AFrostTrap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FrostTrapVentMesh = CreateDefaultSubobject<UStaticMeshComponent>("Frost Trap Vent");
	FrostTrapVentMesh->SetupAttachment(TrapBaseStaticMesh);

	TrapType = ETrapType::FrostTrap;

	HitBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFrostTrap::HandleFrostTrapBeginOverlap);
	HitBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AFrostTrap::HandleFrostTrapEndOverlap);
	
}

// Called when the game starts or when spawned
void AFrostTrap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFrostTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFrostTrap::HandleFrostTrapBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	/* Slow Down Enemy Robots.*/
	ATimberEnemyCharacter* EnemyCharacter = Cast<ATimberEnemyCharacter>(OtherActor);
	if(EnemyCharacter)
	{
		//Adjust Enemy Max Walk/Run Speed.
		EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = 100.f;
	}
}

void AFrostTrap::HandleFrostTrapEndOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ATimberEnemyCharacter* EnemyCharacter = Cast<ATimberEnemyCharacter>(OtherActor);
	if(EnemyCharacter)
	{
		EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = 400.f;
	}
}

