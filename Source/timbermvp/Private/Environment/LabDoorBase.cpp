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

	/* Timeline Animation */
	DoorOpenTimeline = CreateDefaultSubobject<UTimelineComponent>("DoorOpenTimeline");
	
}

// Called when the game starts or when spawned
void ALabDoorBase::BeginPlay()
{
	Super::BeginPlay();

	LabDoorLeftClosePos = LabDoorLeft->GetRelativeLocation();
	LabDoorRightClosePos = LabDoorRight->GetRelativeLocation();

	

	FOnTimelineFloat TimelineProgress;
	//Function that runs on the Timeline Update - Where we will set relative locations of the doors
	TimelineProgress.BindUFunction(this, FName("HandleTimelineProgress"));

	FOnTimelineEvent FinishedFunction;
	FinishedFunction.BindUFunction(this, FName("HandleTimelineFinished"));

	DoorOpenTimeline->AddInterpFloat(FloatCurve, TimelineProgress);
	DoorOpenTimeline->SetTimelineFinishedFunc(FinishedFunction);
	DoorOpenTimeline->SetLooping(false);
	
}

void ALabDoorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALabDoorBase::SetupLabDoorComponents()
{
	LabDoorFrame = CreateDefaultSubobject<UStaticMeshComponent>("LabDoorFrame");
	LabDoorLeft = CreateDefaultSubobject<UStaticMeshComponent>("LabDoorLeft");
	LabDoorRight = CreateDefaultSubobject<UStaticMeshComponent>("LabDoorRight");

	RootComponent = LabDoorFrame;
	LabDoorLeft->SetupAttachment(LabDoorFrame);
	LabDoorRight->SetupAttachment(LabDoorFrame);

	PathTracerStart   = CreateDefaultSubobject<USceneComponent>("PathTracerStart");
	
}

void ALabDoorBase::HandleTimelineProgress(float Value)
{
	float LabDoorLeftOffsetMax = -310.f;
	float LabDoorRightOffsetMax = 270.f;
	
	FVector LabDoorLeftLocation = LabDoorLeftClosePos;
	FVector LabDoorRightLocation = LabDoorRightClosePos;
	
	LabDoorLeft->SetRelativeLocation(LabDoorLeftLocation + FVector(Value * LabDoorLeftOffsetMax,0.f,  0.f));
	LabDoorRight->SetRelativeLocation(LabDoorRightLocation + FVector(Value * LabDoorRightOffsetMax, 0.f, 0.f));
}

void ALabDoorBase::HandleTimelineFinished()
{
	return;
}

void ALabDoorBase::OpenLabDoor(float DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(4, 3.0, FColor::Black, "Opening Lab Door");
	DoorOpenTimeline->PlayFromStart();
}

void ALabDoorBase::CloseLabDoor(float DeltaTime)
{
	DoorOpenTimeline->ReverseFromEnd();
}

