// Property of Paracosm Industries. Dont use my shit.


#include "Character/TimberSeeda.h"

#include "Character/TimberPlayableCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/Inventory/InventoryManagerComponent.h"
#include "Controller/TimberPlayerController.h"
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
	InteractOverlapSphere = CreateDefaultSubobject<UCapsuleComponent>("Interact Sphere");
	InteractOverlapSphere->SetupAttachment(RootComponent);
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

	InteractOverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &ATimberSeeda::AddInteractableToPlayer);
	InteractOverlapSphere->OnComponentEndOverlap.AddDynamic(this, &ATimberSeeda::RemoveInteractableFromPlayer);
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

void ATimberSeeda::RepairSeeda()
{
	UE_LOG(LogTemp, Warning, TEXT("Seeda Repaired."));

	//CheckPlayer Inventory for Repair Items

	ATimberPlayableCharacter* Player = Cast<ATimberPlayableCharacter>(UGameplayStatics::GetActorOfClass
	(GetWorld(), ATimberPlayableCharacter::StaticClass()));

	if (CurrentHealth >= MaxHealth)
	{
		UE_LOG(LogTemp, Warning, TEXT("Seeda is already at Full Health."));
		return;
	}

	if (Player)
	{
		if (Player->InventoryManager)
		{
			int AvailableParts = Player->InventoryManager->GetPartsInInventory();
			if (AvailableParts >= PartsToRepairSeeda)
			{
				Player->InventoryManager->RemovePartsFromInventory(2);
				if (CurrentHealth + HealthAmountGainedOnRepair > MaxHealth)
				{
					CurrentHealth = MaxHealth;
				}
				else
				{
					{
						CurrentHealth += HealthAmountGainedOnRepair;
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Not Enough Parts to Repair Seeda."));
			}
		}
	}
}

void ATimberSeeda::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Seeda Interacted with."));

	RepairSeeda();
}

void ATimberSeeda::AddInteractableToPlayer(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	ATimberPlayableCharacter* PlayerCharacter = Cast<ATimberPlayableCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		ATimberPlayerController* PC = Cast<ATimberPlayerController>(PlayerCharacter->GetController());
		if (PC)
		{
			PC->SetInteractableItem(this);

			//Tells the HUD to show the ToolTip
			OnSeedaInteractOverlap.Broadcast(true);
		}
	}
}

void ATimberSeeda::RemoveInteractableFromPlayer(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ATimberPlayableCharacter* PlayerCharacter = Cast<ATimberPlayableCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		ATimberPlayerController* PC = Cast<ATimberPlayerController>(PlayerCharacter->GetController());
		if (PC)
		{
			PC->ClearInteractableItem();

			//Tells the HUD to hide the ToolTip
			OnSeedaInteractOverlap.Broadcast(false);
		}
	}
}

