// Property of Paracosm Industries. Dont use my shit.


#include "Character/Enemies/TutorialDummy.h"

#include "Components/CapsuleComponent.h"
#include "States/DieRobotGameStateBase.h"


// Sets default values
ATutorialDummy::ATutorialDummy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionCapsule = GetCapsuleComponent();
	if (CollisionCapsule)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tutorial Dummy Collision Capsule Initialized."));
	}
	
}

// Called when the game starts or when spawned
void ATutorialDummy::BeginPlay()
{
	Super::BeginPlay();

	USkeletalMeshComponent* SkeletalMesh = GetMesh();
	if (SkeletalMesh)
	{
		SkeletalMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		
	}
	
	if (CollisionCapsule)
	{
		CollisionCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
		
		//Block By BuildingComponents / Buildables - Ensure it doesnt fall through the map.
		/*CollisionCapsule->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
		CollisionCapsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
		CollisionCapsule->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);*/
		//Projectile
		CollisionCapsule->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
		//Sword
		CollisionCapsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	}

	DieRobotGameStateBase = Cast<ADieRobotGameStateBase>(GetWorld()->GetGameState());

	//Broadcasts to Increment Tutorial State
	OnDestroyed.AddDynamic(this, &ATutorialDummy::HandleDeath);
	
}

void ATutorialDummy::TakeDamage(float DamageAmount, AActor* DamageInstigator)
{
	Super::TakeDamage(DamageAmount, DamageInstigator);
	
	if (CurrentHealth <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tutorial Dummy is Destroyed."));

		Destroy();
	}
	
}

void ATutorialDummy::HandleDeath(AActor* DeadActor)
{
	//Tutorial State Increment to TutorialState::Parts
	if (DieRobotGameStateBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("Switching to Parts Tutorial State."));
		DieRobotGameStateBase->ChangeTutorialGameState(ETutorialState::Parts1);
	}
}


