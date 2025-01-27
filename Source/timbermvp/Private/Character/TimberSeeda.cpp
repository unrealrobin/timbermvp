// Property of Paracosm Industries. Dont use my shit.


#include "Character/TimberSeeda.h"

#include "Character/TimberPlayableCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameModes/TimberGameModeBase.h"
#include "Kismet/GameplayStatics.h"


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

	// Used when Seeda is respawned to allow Listeners to Rebind to any Delegates
	ATimberGameModeBase* GM = Cast<ATimberGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->OnCharacterInitialization.AddDynamic(this, &ATimberSeeda::HandleCharacterBindingToSeeda);
	}

	/*
	 *Typically Seeda waits for the Initialization of the Character to Broadcast to the Character to bind to the Seeda Delegates.
	 * When Seeda is Respawned during Runtime, the Character Initialization is never Broadcast, because the Broadcast occured during the Character's Begin Play.
	 * So Seeda checks on Respawn if the character is valid, and if it is, ensures the character now rebinds to the required delegates.
	 */
	
	ATimberPlayableCharacter* PlayerCharacter = Cast<ATimberPlayableCharacter>(UGameplayStatics::GetActorOfClass(this, ATimberPlayableCharacter::StaticClass()));
	if (PlayerCharacter)
	{
		HandleCharacterBindingToSeeda();
	}
}

// Called every frame
void ATimberSeeda::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATimberSeeda::TakeDamage_Seeda(float DamageAmount)
{
	CurrentHealth -= DamageAmount;
	if (CurrentHealth <= 0)
	{
		OnSeedaDeath.Broadcast(true);
		Destroy();
	}
	
}

void ATimberSeeda::HandleCharacterBindingToSeeda()
{
	ATimberGameModeBase* GM = Cast<ATimberGameModeBase>(GetWorld()->GetAuthGameMode());
	if(GM)
	{
		UE_LOG(LogTemp, Warning, TEXT("Seeda Broadcasts to GameMode"))
		GM->OnSeedaSpawn.Broadcast(this);
	}
}

