// Property of Paracosm Industries. Dont use my shit.


#include "Character/Enemies/Boss/BossBruiser.h"

#include "Components/CapsuleComponent.h"



// Sets default values
ABossBruiser::ABossBruiser()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MaxHealth = 500.0f;
	CurrentHealth = MaxHealth;
	SetupCapsuleComponents();

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
	}
}