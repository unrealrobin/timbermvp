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

	// Setting the lab door open and close positions. These are not dynamic.
	// These vectors are offset amounts from their current location. So from there current positions we want the doors to shift by the vector amount.
	LabDoorLeftOpenPos = FVector(-310.f, 0.f, 0.f);
	LabDoorRightOpenPos = FVector(310.f, 0.f, 0.f);
	LabDoorLeftClosePos = FVector(310.f, 0.f, 0.f);
	LabDoorRightClosePos = FVector(-310.f, 0.f, 0.f);

	

}

// Called when the game starts or when spawned
void ALabDoorBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(LabDoorOpenTimerHandle, this, &ALabDoorBase::HandleTestLabDoorOpen, 5.f, 
	true);
}

// Called every frame
void ALabDoorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(ShouldLabDoorBeOpen && !IsLabDoorOpen)
	{
		OpenLabDoor(DeltaTime);
	}else if(!ShouldLabDoorBeOpen && IsLabDoorOpen)
	{
		CloseLabDoor(DeltaTime);
	}

	
}

void ALabDoorBase::OpenLabDoor(float DeltaTime)
{

	//Using Relative Locations here because the Lab door Locations changes during tick.
	LabDoorLeft->SetRelativeLocation(FMath::VInterpTo(LabDoorLeft->GetRelativeLocation(), LabDoorLeftOpenPos , DeltaTime, 2.f));
	LabDoorRight->SetRelativeLocation(FMath::VInterpTo(LabDoorRight->GetRelativeLocation(), LabDoorRightOpenPos , DeltaTime, 2.f));

	if(LabDoorLeft->GetRelativeLocation().Equals(LabDoorLeftOpenPos, 0.001f) && LabDoorRight->GetRelativeLocation().Equals(LabDoorRightOpenPos, 0.001f))
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::Green, TEXT("Lab Door Opened"));
		IsLabDoorOpen = true;
	}
	
}

void ALabDoorBase::CloseLabDoor(float DeltaTime)
{
	
	//Using Relative Locations here because the Lab door Locations changes during tick.
	LabDoorLeft->SetRelativeLocation(FMath::VInterpTo(LabDoorLeft->GetRelativeLocation(), FVector::ZeroVector , DeltaTime, 2.f));
	LabDoorRight->SetRelativeLocation(FMath::VInterpTo(LabDoorRight->GetRelativeLocation(), FVector::ZeroVector , DeltaTime, 2.f));

	if(LabDoorLeft->GetRelativeLocation().Equals(FVector::ZeroVector, 0.001f)  && LabDoorRight->GetRelativeLocation().Equals(FVector::ZeroVector,0.001f)) 
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::Green, TEXT("Lab Door Closed"));
		IsLabDoorOpen = false;
	}

}



//Used for Debugging Conditions
void ALabDoorBase::SetLabDoorToBeOpen()
{
	ShouldLabDoorBeOpen = true;
}

void ALabDoorBase::SetLabDoorToBeClosed()
{
	ShouldLabDoorBeOpen = false;
}

void ALabDoorBase::HandleTestLabDoorOpen()
{
	GEngine->AddOnScreenDebugMessage(2, 3.0, FColor::Green, TEXT("Handling Lab Door Open"));
	if(IsLabDoorOpen)
	{
		SetLabDoorToBeClosed();
	}else
	{
		SetLabDoorToBeOpen();
	}
}

