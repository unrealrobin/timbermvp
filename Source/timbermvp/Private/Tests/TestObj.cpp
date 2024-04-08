// Property of Paracosm Industries. Dont use my shit.


#include "Tests/TestObj.h"

#include "Components/CapsuleComponent.h"
#include "Character/TimberPlayableCharacter.h"
#include "Controller/TimberPlayerController.h"
#include "Character/Enemies/TimberEnemyCharacter.h"
#include "GameModes/TimberGameModeBase.h"

// Sets default values
ATestObj::ATestObj()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>("CollisionSphere");
	RootComponent = CollisionCapsule;
	StaticMeshBase = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMeshBase->SetupAttachment(CollisionCapsule);
	StaticMeshAttachment = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshAttachment");
	StaticMeshAttachment->SetupAttachment(StaticMeshBase);

}

// Called when the game starts or when spawned
void ATestObj::BeginPlay()
{
	Super::BeginPlay();

	CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &ATestObj::SetInteractItem);
	CollisionCapsule->OnComponentEndOverlap.AddDynamic(this, &ATestObj::UnSetInteractItem);
	InitialLeverRotation = StaticMeshAttachment->GetRelativeRotation();
	
}

// Called every frame
void ATestObj::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestObj::Interact()
{
	//Rotates the Lever to Show it is being interacted with
	if(InitialLeverRotation == StaticMeshAttachment->GetRelativeRotation())
	{
		FRotator NewRotation = LeverOnRotation;
		StaticMeshAttachment->SetRelativeRotation(NewRotation);
	};

	// Calls the GameMode to Spawn the Enemy
	ATimberGameModeBase* GameMode = Cast<ATimberGameModeBase>(GetWorld()->GetAuthGameMode());
	if(GameMode && EnemyToSpawn)
	{
		GameMode->SpawnWave(EnemyToSpawn);
	}
	
	
}

void ATestObj::SetInteractItem(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//TODO: Set this item as the Interactable on the Character Controller

	ATimberPlayableCharacter* TimberCharacter = Cast<ATimberPlayableCharacter>(OtherActor);
	ATimberPlayerController* PlayerController = Cast<ATimberPlayerController>(TimberCharacter->GetController());
	PlayerController->SetInteractableItem(Cast<IInteractable>(this));
	
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, "Overlapped: Set Interactable Item to IIterface");
	}
}

void ATestObj::UnSetInteractItem(
	UPrimitiveComponent*OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if(InitialLeverRotation != StaticMeshAttachment->GetRelativeRotation())
	{
		StaticMeshAttachment->SetRelativeRotation(InitialLeverRotation);
	};
	
	StaticMeshAttachment->AddLocalRotation(FRotator3d(0.0f, 0.0f, 0.0f));
	ATimberPlayableCharacter* TimberCharacter = Cast<ATimberPlayableCharacter>(OtherActor);
	ATimberPlayerController* PlayerController = Cast<ATimberPlayerController>(TimberCharacter->GetController());
	PlayerController->SetInteractableItem(nullptr);
	GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Green, "End Overlap: Set Interactable Item to null");

}

