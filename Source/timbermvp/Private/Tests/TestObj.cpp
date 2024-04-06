// Property of Paracosm Industries. Dont use my shit.


#include "Tests/TestObj.h"

#include "Components/CapsuleComponent.h"
#include "Character/TimberPlayableCharacter.h"
#include "Controller/TimberPlayerController.h"

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
	
}

// Called every frame
void ATestObj::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestObj::Interact()
{
	//TODO: Spawn a Wave of enemies on a spline demo.

	//StaticMeshAttachment->AddRelativeRotation();
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(3, 5.0f, FColor::Green, "TestObj: Interaction function called.");
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
	ATimberPlayableCharacter* TimberCharacter = Cast<ATimberPlayableCharacter>(OtherActor);
	ATimberPlayerController* PlayerController = Cast<ATimberPlayerController>(TimberCharacter->GetController());
	PlayerController->SetInteractableItem(nullptr);
	GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Green, "End Overlap: Set Interactable Item to null");

}

