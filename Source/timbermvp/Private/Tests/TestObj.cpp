// Property of Paracosm Industries. Dont use my shit.


#include "Tests/TestObj.h"

#include "Components/CapsuleComponent.h"
#include "Character/TimberPlayableCharacter.h"
#include "Controller/TimberPlayerController.h"
#include "Subsystems/Wave/WaveGameInstanceSubsystem.h"

// Sets default values
ATestObj::ATestObj()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>("CollisionSphere");
	RootComponent = CollisionCapsule;
	StaticMeshBase = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMeshBase->SetupAttachment(CollisionCapsule);
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
	//Purely Visual
	//Rotates the Lever to Show it is being interacted with
	if (InitialLeverRotation == StaticMeshAttachment->GetRelativeRotation())
	{
		FRotator NewRotation = LeverOnRotation;
		StaticMeshAttachment->SetRelativeRotation(NewRotation);
	}

	{
		GetGameInstance()->GetSubsystem<UWaveGameInstanceSubsystem>()->EarlyStartWave();
		UE_LOG(LogTemp, Warning, TEXT("Game Instance Should Spawn Wave."))
	}
}

void ATestObj::SetInteractItem(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)

{
	ATimberPlayableCharacter* TimberCharacter = Cast<ATimberPlayableCharacter>(OtherActor);
	if (TimberCharacter)
	{
		ATimberPlayerController* PlayerController = Cast<ATimberPlayerController>(TimberCharacter->GetController());
		PlayerController->SetInteractableItem(Cast<IInteractable>(this));
	}
}

void ATestObj::UnSetInteractItem(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (InitialLeverRotation != StaticMeshAttachment->GetRelativeRotation())
	{
		StaticMeshAttachment->SetRelativeRotation(InitialLeverRotation);
	}

	StaticMeshAttachment->AddLocalRotation(FRotator3d(0.0f, 0.0f, 0.0f));
	ATimberPlayableCharacter* TimberCharacter = Cast<ATimberPlayableCharacter>(OtherActor);

	if (TimberCharacter)
	{
		ATimberPlayerController* PlayerController = Cast<ATimberPlayerController>(TimberCharacter->GetController());
		PlayerController->ClearInteractableItem();
	}
}
