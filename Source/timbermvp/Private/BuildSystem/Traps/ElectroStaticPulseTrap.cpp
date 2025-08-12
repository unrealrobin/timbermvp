// Property of Paracosm Industries. 


#include "BuildSystem/Traps/ElectroStaticPulseTrap.h"

#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StatusEffect/StatusConditionManager.h"
#include "Data/DataAssets/StatusEffects/StatusEffectBase.h"
AElectroStaticPulseTrap::AElectroStaticPulseTrap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SnapCondition = ESnapCondition::CenterSnap;
}

void AElectroStaticPulseTrap::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		if (bIsProxy) return;

		FireElectroPulse();
	});
	
	/*if (!bIsProxy)
	{
		FireElectroPulse();
	}*/
}

void AElectroStaticPulseTrap::BeginDestroy()
{
	Super::BeginDestroy();
	if (PulseSphereMeshComponent)
	{
		PulseSphereMeshComponent->DestroyComponent();
	}
}

void AElectroStaticPulseTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsProxy)
	{
		MovePulseHitSphere(DeltaTime);
	}
}

void AElectroStaticPulseTrap::FireElectroPulse()
{
	CreatePulseHitSphere();
}

void AElectroStaticPulseTrap::CreatePulseHitSphere()
{
	/*Adjusting Spawn Location by Radius of Sphere*/
	FVector BaseLocation = BoxExtentRaycastStart->GetComponentLocation();
	FVector ForwardOffset = BoxExtentRaycastStart->GetForwardVector() * SphereRadius;
	//DrawDebugSphere(GetWorld(), BaseLocation + ForwardOffset, 20.0f, 12, FColor::Blue, false, 5.0f);
	
	FVector AdjustedLocation = BaseLocation + ForwardOffset;
	
	PulseSphereHitComponent = NewObject<USphereComponent>(this);
	if (PulseSphereHitComponent)
	{
		/*Sphere CollisionComponent*/
		PulseSphereHitComponent->SetSphereRadius(SphereRadius);
		PulseSphereHitComponent->SetWorldLocation(AdjustedLocation);
		PulseSphereHitComponent->RegisterComponent();
		PulseSphereHitComponent->SetCollisionProfileName(TEXT("HitEventOnly"));
		PulseSphereHitComponent->SetGenerateOverlapEvents(true);
		PulseSphereHitComponent->OnComponentBeginOverlap.AddDynamic(this, &AElectroStaticPulseTrap::HandlePulseSphereOverlap);

		if (PulseSphereMesh)
		{
			//Create the Static Mesh Component of the Pulse
			PulseSphereMeshComponent = NewObject<UStaticMeshComponent>(this);
			PulseSphereMeshComponent->RegisterComponent();
			PulseSphereMeshComponent->AttachToComponent(PulseSphereHitComponent, FAttachmentTransformRules::KeepRelativeTransform);
			PulseSphereMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			PulseSphereMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
			PulseSphereMeshComponent->SetStaticMesh(PulseSphereMesh);
			PulseSphereMeshComponent->SetRelativeScale3D(FVector(0.7f, 0.7f, 0.7f));
		}
	}
}

void AElectroStaticPulseTrap::HandlePulseSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ATimberEnemyCharacter::StaticClass()))
	{
		EffectConditionManager->ResolveEffect(StatusEffectDefinitions, OtherActor);
	}
}

void AElectroStaticPulseTrap::MovePulseHitSphere(float DeltaTime)
{
	if (PulseSphereHitComponent && PulseSphereMeshComponent && PulseSphereMeshComponent->GetStaticMesh())
	{
		//Original Location
		FVector PulseStartLocation = PulseSphereHitComponent->GetComponentLocation();

		//Pulse Direction
		FVector ForwardVector = BoxExtentRaycastStart->GetForwardVector();
		
		//Amount of Translation 
		FVector NewLocation =	PulseStartLocation + (ForwardVector * 200 * DeltaTime);
		
		PulseSphereHitComponent->SetRelativeLocation(NewLocation);

		//Once the Pulse has Reached the End of the Hit Box Extents, Destroy the Pulse Hit Box.
		bool HitEndOfBoxExtent = FVector::Dist(PulseSphereHitComponent->GetComponentLocation(), 
		BoxExtentRaycastStart->GetComponentLocation()) >= (CalculatedBoxLength - SphereRadius);

		if ( HitEndOfBoxExtent )
		{
			//The actual box that causes the Overlaps/Hits
			PulseSphereHitComponent->DestroyComponent();
			PulseSphereHitComponent = nullptr;

			//We don't set this to nullptr as we define the shape in the editor
			PulseSphereMeshComponent->DestroyComponent();

			FTimerHandle CooldownTimer;
			TWeakObjectPtr<AElectroStaticPulseTrap> WeakThis = this;
			GetWorld()->GetTimerManager().SetTimer(CooldownTimer, [WeakThis]()
			{
				if (WeakThis.IsValid())
				{
					WeakThis->FireElectroPulse();
				}
			}, 1.0f, false);
			
			//UE_LOG(LogTemp, Warning, TEXT("PulseHitBox Destroyed"));
		}
	}
}




