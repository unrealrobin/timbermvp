// Property of Paracosm.


#include "BuildSystem/Traps/BurnTrap.h"

#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Data/DataAssets/StatusEffects/StatusEffectBase.h"


// Sets default values
ABurnTrap::ABurnTrap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SnapCondition = ESnapCondition::CenterSnap;
	
	BurnTrapInternalsMesh = CreateDefaultSubobject<UStaticMeshComponent>("BurnTrapInternalsMesh");
	BurnTrapInternalsMesh->SetupAttachment(TrapBaseStaticMesh);

	BurnTrapInternalsNiagara = CreateDefaultSubobject<UNiagaraComponent>("BurnTrapInternalsNiagara");
	BurnTrapInternalsNiagara->SetupAttachment(BurnTrapInternalsMesh);
	
	HitBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABurnTrap::HandleTrapBeginOverlap);
	HitBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ABurnTrap::HandleTrapEndOverlap);
	
}

// Called when the game starts or when spawned
void ABurnTrap::BeginPlay()
{
	Super::BeginPlay();
}

void ABurnTrap::HandleTrapBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (BurnTrapInternalsNiagara)
	{
		BurnTrapInternalsNiagara->ActivateSystem();
	}
	
	if (StatusEffectDataAsset)
	{
		AddEffectToEnemy(OtherActor, StatusEffectDataAsset->StatusEffect);
	}
}

void ABurnTrap::HandleTrapEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 INT32)
{
	//Check if anyone is still in hitbox, if not, turn off the effect.
	if (InsideHitBoxArray.Num() == 0)
	{
		if (BurnTrapInternalsNiagara)
		{
			BurnTrapInternalsNiagara->Deactivate();
			UE_LOG(LogTemp, Display, TEXT("Burn Trap - Deactivated Niagara System."));
		}
	}
}

// Called every frame
void ABurnTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

