// Property of Paracosm Industries. 


#include "BuildSystem/Traps/ElectroStaticPulseTrap.h"

#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Components/BoxComponent.h"



AElectroStaticPulseTrap::AElectroStaticPulseTrap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SnapCondition = ESnapCondition::CenterSnap;
}

void AElectroStaticPulseTrap::BeginPlay()
{
	Super::BeginPlay();

	//Continuously Fire the Electro-Pulse ever X Seconds
	if (!bIsProxy)
	{
		GetWorld()->GetTimerManager().SetTimer(FireElectroPulseTimerHandle, this,  &AElectroStaticPulseTrap::FireElectroPulse, FireElectroPulseCooldown, false );
	}
}

void AElectroStaticPulseTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovePulseHitBox(DeltaTime);
}

void AElectroStaticPulseTrap::FireElectroPulse()
{
	CreatePulseHitBox();
}

void AElectroStaticPulseTrap::CreatePulseHitBox()
{
	//Spawns a Collision Box
	PulseHitBox = NewObject<UBoxComponent>(this);
	if (PulseHitBox)
	{
		//UE_LOG(LogTemp, Warning, TEXT("PulseHitBox Created"));
		//Spawn the Box at the BoxExtentRayCast Start
		PulseHitBox->SetRelativeLocation(BoxExtentRaycastStart->GetComponentLocation());

		FRotator AdjustedRotation = TrapBaseStaticMesh->GetComponentRotation();
		
		PulseHitBox->SetRelativeRotation(AdjustedRotation);
		
		PulseHitBox->RegisterComponent();

		//For other size Traps these variables may need to be changed.
		PulseHitBox->SetBoxExtent(FVector(BoxWidth, 20.f, BoxHeight));
		PulseHitBox->SetCollisionProfileName(TEXT("HitEventOnly"));
		
		//Collision Delegate
		PulseHitBox->OnComponentBeginOverlap.AddDynamic(this, &AElectroStaticPulseTrap::HandlePulseBoxOverlap);
	}

	if (PulseHitBox && PulseMesh)
	{
		//Create the Static Mesh Component of the Pulse
		PulseMeshComponent = NewObject<UStaticMeshComponent>(this);
		
		PulseMeshComponent->RegisterComponent();
		
		PulseMeshComponent->AttachToComponent(PulseHitBox, FAttachmentTransformRules::KeepRelativeTransform);
		
		//PulseMeshComponent->SetWorldLocation(PulseHitBox->GetComponentLocation());

		//Pulse Mesh is purely a visual representation of the Pulse Hit Box
		PulseMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		
		//Setting the referenced Mesh as the mesh for this component.
		PulseMeshComponent->SetStaticMesh(PulseMesh);
	}
}

void AElectroStaticPulseTrap::MovePulseHitBox(float DeltaTime)
{
	if (PulseHitBox && PulseMeshComponent && PulseMeshComponent->GetStaticMesh())
	{
		//Original Location
		FVector PulseStartLocation = PulseHitBox->GetComponentLocation();

		//Pulse Direction
		FVector ForwardVector = BoxExtentRaycastStart->GetForwardVector();
		
		//Amount of Translation 
		FVector NewLocation =	PulseStartLocation + (ForwardVector * 200 * DeltaTime);
		
		PulseHitBox->SetRelativeLocation(NewLocation);

		//Once the Pulse has Reached the End of the Hit Box Extents, Destroy the Pulse Hit Box.
		bool HitEndOfBoxExtent = FVector::Dist(PulseHitBox->GetComponentLocation(), 
		BoxExtentRaycastStart->GetComponentLocation()) >= CalculatedBoxLength;

		if ( HitEndOfBoxExtent )
		{
			//The actual box that causes the Overlaps/Hits
			PulseHitBox->DestroyComponent();
			PulseHitBox = nullptr;

			//We don't set this to nullptr as we define the shape in the editor
			PulseMeshComponent->DestroyComponent();

			//Continuously Fire the Electro-Pulse ever @PARAM: FireElectroPulseCooldown Seconds
			GetWorld()->GetTimerManager().SetTimer(FireElectroPulseTimerHandle, this,  
			&AElectroStaticPulseTrap::FireElectroPulse, FireElectroPulseCooldown, false );
			
			//UE_LOG(LogTemp, Warning, TEXT("PulseHitBox Destroyed"));
		}
	}
}

void AElectroStaticPulseTrap::HandlePulseBoxOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	ATimberEnemyCharacter* Enemy = Cast<ATimberEnemyCharacter>(OtherActor);

	//UE_LOG(LogTemp, Warning, TEXT("HandlePulseBoxOverlap - %ls"), *OtherActor->GetName());

	//Apply Initial Damage to Enemy.
	if (Enemy)
	{
		Enemy->TakeDamage(InitialHitDamage, this);
	}

	//TODO::Apply DOT on Enemy that last 10 Seconds doing 2 damage every other second.
	
}



