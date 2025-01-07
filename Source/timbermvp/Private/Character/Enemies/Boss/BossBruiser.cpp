// Property of Paracosm Industries. Dont use my shit.


#include "Character/Enemies/Boss/BossBruiser.h"

#include "AudioDevice.h"
#include "Character/TimberPlayableCharacter.h"
#include "Components/CapsuleComponent.h"


// Sets default values
ABossBruiser::ABossBruiser()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CurrentHealth = MaxHealth;
	SetupCapsuleComponents();

	WhirlwindRightCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ABossBruiser::HandleWhirlwindOverlap);
	WhirlwindLeftCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ABossBruiser::HandleWhirlwindOverlap);
	RightArmCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ABossBruiser::HandleBHandSlapOverlap);
}

// Called when the game starts or when spawned
void ABossBruiser::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABossBruiser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABossBruiser::EnableCollisionToDamagePlayerOnly(UCapsuleComponent* WWCapsuleComponent)
{
	if (WWCapsuleComponent)
	{
		WWCapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		WWCapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
}

void ABossBruiser::DisableCollisionToDamagePlayerOnly(UCapsuleComponent* WWCapsuleComponent)
{
	if (WWCapsuleComponent)
	{
		WWCapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	}
}

void ABossBruiser::HandleWhirlwindOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	ATimberPlayableCharacter* PlayerCharacter = Cast<ATimberPlayableCharacter>(OtherActor);

	//Only damages the Player, will not Damage other enemies or Building Components.
	if (PlayerCharacter)
	{
		PlayerCharacter->PlayerTakeDamage(WhirlwindTickDamage);
	}
}

void ABossBruiser::HandleBHandSlapOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	ATimberPlayableCharacter* PlayerCharacter = Cast<ATimberPlayableCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->PlayerTakeDamage(BHandSlapDamage);
	}
}

void ABossBruiser::HandleOverHeadSmashOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OverHeadSmashOverlap"));
	ATimberPlayableCharacter* PlayerCharacter = Cast<ATimberPlayableCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->PlayerTakeDamage(OverHeadSmashDamage);
	}
}

void ABossBruiser::SpawnOverHeadCapsule()
{
	OverHeadSmashCapsuleComponent = NewObject<UCapsuleComponent>(this);
	if (OverHeadSmashCapsuleComponent)
	{
		OverHeadSmashCapsuleComponent->AttachToComponent(
			OverHeadSmashCapsuleSpawnLocation,
			FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		OverHeadSmashCapsuleComponent->RegisterComponent();
		OverHeadSmashCapsuleComponent->SetCapsuleHalfHeight(OverHeadSmashCapsuleHeight, true);
		OverHeadSmashCapsuleComponent->SetCapsuleRadius(OverHeadSmashCapsuleRadius, true);
		//Collision Delegate
		OverHeadSmashCapsuleComponent->OnComponentBeginOverlap.AddDynamic(
			this, &ABossBruiser::HandleOverHeadSmashOverlap);

		//TODO:: May need to be more stringent with the collision setting. This should only damage players within the AOE of the attack.
		OverHeadSmashCapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		OverHeadSmashCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		OverHeadSmashCapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
}

void ABossBruiser::DestroyOverHeadCapsule()
{
	if (OverHeadSmashCapsuleComponent)
	{
		OverHeadSmashCapsuleComponent->DestroyComponent();
	}
}

void ABossBruiser::SetupCapsuleComponents()
{
	if (GetMesh())
	{
		//Used for HeadShotCollisions
		HeadCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("HeadCapsuleComponent");
		HeadCapsuleComponent->SetupAttachment(GetMesh(), FName("headSocket"));

		//Used only for attack animation collision
		RightArmCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("RightArmCapsuleComponent");
		LeftArmCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("LeftArmCapsuleComponent");
		RightArmCapsuleComponent->SetupAttachment(GetMesh(), FName("rightArmSocket"));
		LeftArmCapsuleComponent->SetupAttachment(GetMesh(), FName("leftArmSocket"));

		//Capsules for Whirldwind Animation
		WhirlwindRightCollisionSphere = CreateDefaultSubobject<UCapsuleComponent>("WhirlwindRightCollisionSphere");
		WhirlwindLeftCollisionSphere = CreateDefaultSubobject<UCapsuleComponent>("WhirlwindLeftCollisionSphere");
		WhirlwindRightCollisionSphere->SetupAttachment(RightArmCapsuleComponent);
		WhirlwindLeftCollisionSphere->SetupAttachment(LeftArmCapsuleComponent);
		//Collision Enabling will happen in the Event Notify Custom Event Function
		DisableCollisionToDamagePlayerOnly(WhirlwindRightCollisionSphere);
		DisableCollisionToDamagePlayerOnly(WhirlwindLeftCollisionSphere);

		//Creating Scene Component To Spawn Overhead Smash Location
		OverHeadSmashCapsuleSpawnLocation = CreateDefaultSubobject<
			USceneComponent>("OverHeadSmashCapsuleSpawnLocation");
		OverHeadSmashCapsuleSpawnLocation->SetupAttachment(GetRootComponent());
	}
}
