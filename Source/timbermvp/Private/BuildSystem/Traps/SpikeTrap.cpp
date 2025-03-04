// Property of Paracosm Industries. Dont use my shit.


#include "BuildSystem/Traps/SpikeTrap.h"

#include "Character/TimberPlayableCharacter.h"
#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"


// Sets default values
ASpikeTrap::ASpikeTrap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TrapSpikeMesh = CreateDefaultSubobject<UStaticMeshComponent>("Spikes");
	TrapSpikeMesh->SetupAttachment(RootComponent);
	DisableAllStaticMeshCollisions(TrapSpikeMesh);
	SpikeOutTimeline = CreateDefaultSubobject<UTimelineComponent>("SpikeOutTimeline");
}

void ASpikeTrap::BeginPlay()
{
	Super::BeginPlay();
	HitBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASpikeTrap::HandleSpikeTrapOverlap);
	SetupTimelineData();
	FVector SpikeStartScale = FVector(0.1f, 1.0f, 1.0f);
	TrapSpikeMesh->SetRelativeScale3D(SpikeStartScale);
}

// Called every frame
void ASpikeTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	{ //Enemies pile up but don't Begin Overlap within the spike trap, so we force the spike out attack if the Trap is off cooldown.
		if(InsideHitBoxArray.Num() != 0 && !IsSpikeOnCooldown)
		{
			SetSpikeOutAttackTimer();
		}
	}
}

void ASpikeTrap::SetSpikeOutAttackTimer()
{
	GetWorld()->GetTimerManager().SetTimer(
		TimeToActiveSpikeOutAttack, this, &ASpikeTrap::HandleSpikeOutAttack,
		TimeToActiveSpikeOutAttackValue, false);
	IsSpikeOnCooldown = true;
}

void ASpikeTrap::HandleSpikeTrapOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent*
	OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Timer from initial activation to spike out attack
	ATimberEnemyCharacter* Enemy = Cast<ATimberEnemyCharacter>(OtherActor);
	if (Enemy)
	{
		if (!IsSpikeOnCooldown) // IF the spike is not on cooldown, then we can activate the spike out attack.
		{
			SetSpikeOutAttackTimer();
		}
	}
}

void ASpikeTrap::HandleSpikeOutAttack()
{
	PlaySpikeOutTimeline();
	ApplyDamageToActorsInHitBox();
}

void ASpikeTrap::ApplyDamageToActorsInHitBox()
{
	if (InsideHitBoxArray.Num() > 0)
	{
		TArray<AActor*> InsideHitBoxArrayCopy = InsideHitBoxArray;
		for (AActor* Actors : InsideHitBoxArrayCopy)
		{
			if (ATimberEnemyCharacter* EnemyCharacter = Cast<ATimberEnemyCharacter>(Actors))
			{
				
				EnemyCharacter->TakeDamage(SpikeDamage, this);
				//GEngine->AddOnScreenDebugMessage(6, 2, FColor::Red, "Damage");
				UE_LOG(LogTemp, Display, TEXT("Damage: %f"), SpikeDamage);
			}
		}
	}
}

/*Timeline Animations*/
void ASpikeTrap::SetupTimelineComponents()
{
	SpikeOutTimeline = CreateDefaultSubobject<UTimelineComponent>("SpikeOutTimeline");
}

void ASpikeTrap::SetupTimelineData()
{
	if (SpikeAnimVectorCurve)
	{
		//Called every frame while timeline is running.
		FOnTimelineVector SpikeOutProgress;
		//Binding this callback function to this Timeline
		SpikeOutProgress.BindUFunction(this, FName("HandleVectorCurveUpdate"));

		FOnTimelineEvent SpikeOutFinished;
		SpikeOutFinished.BindUFunction(this, FName("SpikeOutTimelineFinished"));
		//Takes a vector and an event to bind to the timeline
		SpikeOutTimeline->AddInterpVector(SpikeAnimVectorCurve, SpikeOutProgress);
		//Takes an event to bind to the timeline finishing. The event then calls the function that is bound to it. (SpikeOutTimelineFinished())
		SpikeOutTimeline->SetTimelineFinishedFunc(SpikeOutFinished);
		SpikeOutTimeline->SetLooping(false);
	}
}

void ASpikeTrap::HandleVectorCurveUpdate(FVector CurveVector)
{
	//where you define what happens every frame, such as changing scale, position, or material.
	if (TrapSpikeMesh)
	{
		TrapSpikeMesh->SetRelativeScale3D(CurveVector);
	}
}

void ASpikeTrap::SpikeOutTimelineFinished()
{
	if (IsSpikesOut) // This is what is called when the Spike Anim unleashes the spikes as the end function.
	{
		PlaySpikeOutTimeline_Reverse();
	}
	else // This is what is called when the Spike Anim retracts the spikes as the end function.
	{
		IsSpikesOut = false;
		//Timer Initiates a Cool down before the Spike Attack can Happen again.
		GetWorld()->GetTimerManager().SetTimer(
			SpikeOutCooldown, this, &ASpikeTrap::EndSpikeTrapCooldown, SpikeOutCooldownValue,
			false);
	}
}

void ASpikeTrap::PlaySpikeOutTimeline()
{
	if (SpikeOutTimeline)
	{
		SpikeOutTimeline->PlayFromStart();
		IsSpikesOut = true;
	}
}

void ASpikeTrap::PlaySpikeOutTimeline_Reverse()
{
	SpikeOutTimeline->ReverseFromEnd();
	IsSpikesOut = false;
}

void ASpikeTrap::EndSpikeTrapCooldown()
{
	IsSpikeOnCooldown = false;
	UE_LOG(LogTemp, Warning, TEXT("Spike Cooldown Finished."));
}
