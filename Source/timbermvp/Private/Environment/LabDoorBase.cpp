// Property of Paracosm Industries. Dont use my shit.


#include "Environment/LabDoorBase.h"

#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
ALabDoorBase::ALabDoorBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetupLabDoorComponents();
}

// Called when the game starts or when spawned
void ALabDoorBase::BeginPlay()
{
	Super::BeginPlay();
	InitializeLabDoorProperties();
}

// Called every frame
void ALabDoorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ShouldLabDoorBeOpen && !IsLabDoorOpen)
	{
		OpenLabDoor(DeltaTime);
	}
	else if (!ShouldLabDoorBeOpen && IsLabDoorOpen)
	{
		CloseLabDoor(DeltaTime);
	}
}

void ALabDoorBase::SetupLabDoorComponents()
{
	LabDoorFrame = CreateDefaultSubobject<UStaticMeshComponent>("LabDoorFrame");
	LabDoorLeft = CreateDefaultSubobject<UStaticMeshComponent>("LabDoorLeft");
	LabDoorRight = CreateDefaultSubobject<UStaticMeshComponent>("LabDoorRight");

	RootComponent = LabDoorFrame;
	LabDoorLeft->SetupAttachment(LabDoorFrame);
	LabDoorRight->SetupAttachment(LabDoorFrame);

	DoorOpenTimeline = CreateDefaultSubobject<UTimelineComponent>("DoorOpenTimeline");

	/* TO BE DELETED BELOW*/
	
	// Setting the lab door open and close positions. These are not dynamic.
	// These vectors are offset amounts from their current location. So from there current positions we want the doors to shift by the vector amount.
	LabDoorLeftOpenPos = FVector(-310.f, 0.f, 0.f);
	LabDoorRightOpenPos = FVector(310.f, 0.f, 0.f);
	LabDoorLeftClosePos = FVector(310.f, 0.f, 0.f);
	LabDoorRightClosePos = FVector(-310.f, 0.f, 0.f);

	LabDoorActivatorComponent = CreateDefaultSubobject<UBoxComponent>("LabDoorActivatorComponent");
	LabDoorActivatorComponent->SetupAttachment(RootComponent);
	//LabDoorActivatorComponent->OnComponentBeginOverlap.AddDynamic(this, &ALabDoorBase::HandleOpenLabDoor);
	//LabDoorActivatorComponent->OnComponentEndOverlap.AddDynamic(this, &ALabDoorBase::HandleCloseLabDoor);
}

void ALabDoorBase::OpenLabDoor(float DeltaTime)
{
	//Using Relative Locations here because the Lab door Locations changes during tick.
	LabDoorLeft->SetRelativeLocation(
		FMath::VInterpConstantTo(LabDoorLeft->GetRelativeLocation(), LabDoorLeftOpenPos, DeltaTime, 155.f));
	LabDoorRight->SetRelativeLocation(
		FMath::VInterpConstantTo(LabDoorRight->GetRelativeLocation(), LabDoorRightOpenPos, DeltaTime, 155.f));

	if (LabDoorLeft->GetRelativeLocation().Equals(LabDoorLeftOpenPos, 1.0f) && LabDoorRight->GetRelativeLocation().
		Equals(LabDoorRightOpenPos, 1.0f))
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::Green, TEXT("Lab Door Opened"));
		IsLabDoorOpen = true;
	}
}

void ALabDoorBase::CloseLabDoor(float DeltaTime)
{
	//Using Relative Locations here because the Lab door Locations changes during tick.
	LabDoorLeft->SetRelativeLocation(
		FMath::VInterpConstantTo(LabDoorLeft->GetRelativeLocation(), FVector::ZeroVector, DeltaTime, 155.f));
	LabDoorRight->SetRelativeLocation(
		FMath::VInterpConstantTo(LabDoorRight->GetRelativeLocation(), FVector::ZeroVector, DeltaTime, 155.f));

	if (LabDoorLeft->GetRelativeLocation().Equals(FVector::ZeroVector, 1.0f) && LabDoorRight->GetRelativeLocation().
		Equals(FVector::ZeroVector, 1.0f))
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::Green, TEXT("Lab Door Closed"));
		IsLabDoorOpen = false;
	}
}

void ALabDoorBase::HandleOpenLabDoor(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	ATimberEnemyCharacter* TimberEnemyCharacter = Cast<ATimberEnemyCharacter>(OtherActor);
	if (TimberEnemyCharacter)
	{
		EnemiesInLabDoorActivator.Add(TimberEnemyCharacter);
		SetLabDoorToBeOpen();
	}
}

void ALabDoorBase::HandleCloseLabDoor(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ATimberEnemyCharacter* TimberEnemyCharacter = Cast<ATimberEnemyCharacter>(OtherActor);
	if (EnemiesInLabDoorActivator.Contains(TimberEnemyCharacter))
	{
		EnemiesInLabDoorActivator.Remove(TimberEnemyCharacter);
		if (EnemiesInLabDoorActivator.IsEmpty())
		{
			SetLabDoorToBeClosed();
		}
	}
}

void ALabDoorBase::SetLabDoorToBeOpen()
{
	ShouldLabDoorBeOpen = true;
}

void ALabDoorBase::SetLabDoorToBeClosed()
{
	ShouldLabDoorBeOpen = false;
}

//Used for Debugging Conditions
void ALabDoorBase::HandleTestLabDoorOpen()
{
	GEngine->AddOnScreenDebugMessage(2, 3.0, FColor::Green, TEXT("Handling Lab Door Open"));
	if (IsLabDoorOpen)
	{
		SetLabDoorToBeClosed();
	}
	else
	{
		SetLabDoorToBeOpen();
	}
}

void ALabDoorBase::InitializeLabDoorProperties()
{
	// Set initial positions of the doors to their closed positions
	LabDoorLeft->SetRelativeLocation(FVector::ZeroVector);
	LabDoorRight->SetRelativeLocation(FVector::ZeroVector);
	ShouldLabDoorBeOpen = false;
	IsLabDoorOpen = false;
	EnemiesInLabDoorActivator.Empty();
}
