// Property of Paracosm Industries. Dont use my shit.


#include "BuildSystem/Traps/ElectroStaticPulseTrap.h"

#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Components/BoxComponent.h"


// Sets default values
AElectroStaticPulseTrap::AElectroStaticPulseTrap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AElectroStaticPulseTrap::BeginPlay()
{
	Super::BeginPlay();

	//Continuously Fire the Electro-Pulse ever X Seconds
	GetWorld()->GetTimerManager().SetTimer(FireElectroPulseTimerHandle, this,  
	&AElectroStaticPulseTrap::FireElectroPulse, FireElectroPulseCooldown, true );
}

// Called every frame
void AElectroStaticPulseTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PulseHitBox)
	{
		//Original Location
		FVector PulseStartLocation = PulseHitBox->GetComponentLocation();

		//Pulse Direction
		FVector ForwardVector = BoxExtentRaycastStart->GetRightVector();
		
		//Amount of Translation 
		FVector NewLocation =	PulseStartLocation + (ForwardVector * 200 * DeltaTime);
		
		PulseHitBox->SetRelativeLocation(NewLocation);

		//Once the Pulse has Reached the End of the Hit Box Extents, Destroy the Pulse Hit Box.
		bool HitEndOfBoxExtent = FVector::Dist(PulseHitBox->GetComponentLocation(), 
		BoxExtentRaycastStart->GetComponentLocation()) >= MaxHitBoxLength;

		if ( HitEndOfBoxExtent )
		{
			PulseHitBox->DestroyComponent();
			PulseHitBox = nullptr;
			UE_LOG(LogTemp, Warning, TEXT("PulseHitBox Destroyed"));
		}
	}
}

void AElectroStaticPulseTrap::FireElectroPulse()
{
	//Spawns a Collision Box
	PulseHitBox = NewObject<UBoxComponent>(this);
	if (PulseHitBox)
	{
		UE_LOG(LogTemp, Warning, TEXT("PulseHitBox Created"));
		//Spawn the Box at the BoxExtentRayCast Start
		PulseHitBox->SetRelativeLocation(BoxExtentRaycastStart->GetComponentLocation());
		PulseHitBox->SetRelativeRotation(BoxExtentRaycastStart->GetComponentRotation());
		
		PulseHitBox->RegisterComponent();

		//For other size Traps these variables may need to be changed.
		PulseHitBox->SetBoxExtent(FVector(BoxWidth, 20.f, BoxHeight));
		PulseHitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
		PulseHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		PulseHitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		PulseHitBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
		PulseHitBox->SetHiddenInGame(false);
		
		//Collision Delegate
		PulseHitBox->OnComponentBeginOverlap.AddDynamic(this, &AElectroStaticPulseTrap::HandlePulseBoxOverlap);

	}
	
	//Move the collision Box from Start to End of HitBoxComponent Length
	
	
}

void AElectroStaticPulseTrap::HandlePulseBoxOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	ATimberEnemyCharacter* Enemy = Cast<ATimberEnemyCharacter>(OtherActor);

	//Apply Initial Damage to Enemy.
	if (Enemy)
	{
		Enemy->TakeDamage(InitialHitDamage, this);
	}

	//Apply DOT on Enemy that last 10 Seconds doing 2 damage every other second.
	
}



