// Property of Paracosm Industries. Dont use my shit.


#include "Environment/LabDoorBase.h"

// Sets default values
ALabDoorBase::ALabDoorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LabDoorFrame = CreateDefaultSubobject<UStaticMeshComponent>("LabDoorFrame");
	LabDoorLeft = CreateDefaultSubobject<UStaticMeshComponent>("LabDoorLeft");
	LabDoorRight = CreateDefaultSubobject<UStaticMeshComponent>("LabDoorRight");

	LabDoorFrame->SetupAttachment(RootComponent);
	LabDoorLeft->SetupAttachment(LabDoorFrame);
	LabDoorRight->SetupAttachment(LabDoorFrame);

	LabDoorLeftClosePos = LabDoorLeft->GetComponentLocation();
	LabDoorRightClosePos = LabDoorRight->GetComponentLocation();

}

// Called when the game starts or when spawned
void ALabDoorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALabDoorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALabDoorBase::OpenLabDoor()
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	FVector LabDoorLeftOpenPosition = FVector::ZeroVector;
	LabDoorLeftOpenPosition.X = -310.f;
	FVector LabDoorRightOpenPosition = FVector::ZeroVector;
	LabDoorRightOpenPosition.X = 310.f;
	
	LabDoorLeftOpenPos = FMath::VInterpTo(LabDoorLeftClosePos, LabDoorLeftOpenPosition , DeltaTime, 2.f);
	LabDoorRightOpenPos = FMath::VInterpTo(LabDoorRightClosePos, LabDoorRightOpenPosition , DeltaTime, 2.f);

	LabDoorLeft->SetRelativeLocation(LabDoorLeftOpenPos);
	LabDoorRight->SetRelativeLocation(LabDoorRightOpenPos);
}

void ALabDoorBase::CloseLabDoor()
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	//TODO::  How Can we verify that these locations exists and were set in OpenLabDoor() before calling these two functions?
	LabDoorLeft->SetRelativeLocation(FMath::VInterpTo(LabDoorLeftOpenPos, LabDoorLeftClosePos , DeltaTime, 1.f));
	LabDoorLeft->SetRelativeLocation(FMath::VInterpTo(LabDoorRightOpenPos, LabDoorRightClosePos , DeltaTime, 1.f));

	
}

