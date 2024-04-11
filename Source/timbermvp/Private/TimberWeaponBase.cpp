// Property of Paracosm Industries. Dont use my shit.


#include "TimberWeaponBase.h"

#include "Components/CapsuleComponent.h"

// Sets default values
ATimberWeaponBase::ATimberWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	RootComponent = CapsuleComponent;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(CapsuleComponent);

}

// Called when the game starts or when spawned
void ATimberWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATimberWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

