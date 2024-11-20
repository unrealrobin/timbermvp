// Property of Paracosm Industries. Dont use my shit.


#include "Character/Enemies/Boss/BossBruiser.h"

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
	if(WWCapsuleComponent)
	{
		WWCapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		WWCapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
	
	
}

void ABossBruiser::DisableCollisionToDamagePlayerOnly(UCapsuleComponent* WWCapsuleComponent)
{
	if(WWCapsuleComponent)
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
	if(PlayerCharacter)
	{
		PlayerCharacter->PlayerTakeDamage(WhirlwindTickDamage);
	}
}

void ABossBruiser::SetupCapsuleComponents()
{
	if(GetMesh())
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
	}
}
