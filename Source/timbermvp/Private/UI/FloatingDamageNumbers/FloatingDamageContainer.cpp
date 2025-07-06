// Property of Paracosm.


#include "UI/FloatingDamageNumbers/FloatingDamageContainer.h"

#include "Components/WidgetComponent.h"
#include "UI/FloatingDamageNumbers/DamageNumberWidget.h"


// Sets default values
AFloatingDamageContainer::AFloatingDamageContainer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");
	RootComponent = RootSceneComponent;
	
	DamageNumberWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("DamageNumberWidgetComponent");
	DamageNumberWidgetComponent->SetupAttachment(RootComponent);
	
}

void AFloatingDamageContainer::SetDamageAmount(float DamageAmount)
{
	if (DamageNumberWidgetComponent)
	{
		UUserWidget* DamageNumberWidget = DamageNumberWidgetComponent->GetWidget();
		if (UDamageNumberWidget* DamageNumberWidgetCast = Cast<UDamageNumberWidget>(DamageNumberWidget))
		{
			DamageNumberWidgetCast->DamageNumber = DamageAmount;
		}
		
	}
}

void AFloatingDamageContainer::HandleDestroy()
{
	Destroy();
}

// Called when the game starts or when spawned
void AFloatingDamageContainer::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();

	//Directly over Spawn Location with Vertical Rise Offset.
	EndLocation = StartLocation + FVector(0, 0, VerticalRiseAmount);
	
	FTimerHandle DamageNumberTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DamageNumberTimerHandle, this, &AFloatingDamageContainer::HandleDestroy, TimeUntilDestroy, false);
	
}

// Called every frame
void AFloatingDamageContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;

	//Can be used Directly for Linear Interpolation.
	float Alpha = FMath::Clamp(ElapsedTime / TimeUntilDestroy, 0.0f, 1.0f);

	//Used for Bounce Effect.
	float BounceAlpha = FMath::InterpEaseOut(0.f, 1.f, Alpha, 4.0f);

	FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, BounceAlpha);
	SetActorLocation(NewLocation);
	
}

