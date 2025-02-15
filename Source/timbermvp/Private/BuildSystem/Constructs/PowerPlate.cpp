// Property of Paracosm Industries. Dont use my shit.


#include "BuildSystem/Constructs/PowerPlate.h"

#include "Character/TimberPlayableCharacter.h"
#include "Components/BoxComponent.h"


// Sets default values
APowerPlate::APowerPlate()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");
	RootComponent = RootSceneComponent;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMeshComponent->SetupAttachment(RootComponent);
	CollisionBox = CreateDefaultSubobject<UBoxComponent>("CollisionBox");
	CollisionBox->SetupAttachment(RootComponent);
	
	
}

// Called when the game starts or when spawned
void APowerPlate::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APowerPlate::HitBoxBeginOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &APowerPlate::HitBoxEndOverlap);
	
}

void APowerPlate::HitBoxBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("PowerPlate HitBoxBeginOverlap"));

	ATimberPlayableCharacter* TimberCharacter = Cast<ATimberPlayableCharacter>(OtherActor);
	if (TimberCharacter)
	{
		//Adding 50% Damage Increase
		TimberCharacter->DamageModifierValue += .5f;
	}
}

void APowerPlate::HitBoxEndOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("PowerPlate HitBoxEndOverlap"));

	ATimberPlayableCharacter* TimberCharacter = Cast<ATimberPlayableCharacter>(OtherActor);
	if (TimberCharacter)
	{
		//Removing 50% Damage Increase
		TimberCharacter->DamageModifierValue -= .5f;
	}
}

// Called every frame
void APowerPlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

