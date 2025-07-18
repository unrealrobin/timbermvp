// Property of Paracosm.


#include "timbermvp/Public/UI/FloatingData/FloatingDataContainer.h"

#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Components/WidgetComponent.h"
#include "timbermvp/Public/UI/FloatingData/FloatingDataWidget.h"


// Sets default values
AFloatingDataContainer::AFloatingDataContainer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");
	RootComponent = RootSceneComponent;
	
	DamageNumberWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("DamageNumberWidgetComponent");
	DamageNumberWidgetComponent->SetupAttachment(RootComponent);
	
}

void AFloatingDataContainer::SetIsDamage(bool bIsThisDamage)
{
	if (DamageNumberWidgetComponent)
	{
		UUserWidget* DamageNumberWidget = DamageNumberWidgetComponent->GetWidget();
		if (UFloatingDataWidget* DamageNumberWidgetCast = Cast<UFloatingDataWidget>(DamageNumberWidget))
		{
			DamageNumberWidgetCast->bIsDamage = bIsThisDamage;
		}
	}
}

void AFloatingDataContainer::SetEffectText(FName EffectName)
{
	if (DamageNumberWidgetComponent)
	{
		UUserWidget* DamageNumberWidget = DamageNumberWidgetComponent->GetWidget();
		if (UFloatingDataWidget* DamageNumberWidgetCast = Cast<UFloatingDataWidget>(DamageNumberWidget))
		{
			DamageNumberWidgetCast->EffectName = EffectName;
		}
	}
}

void AFloatingDataContainer::SetDamageAmount(float InDamageAmount)
{
	if (DamageNumberWidgetComponent)
	{
		UUserWidget* DamageNumberWidget = DamageNumberWidgetComponent->GetWidget();
		if (UFloatingDataWidget* DamageNumberWidgetCast = Cast<UFloatingDataWidget>(DamageNumberWidget))
		{
			DamageNumberWidgetCast->DamageNumber += InDamageAmount; 
			//DamageNumberWidgetCast->DamageNumber = InDamageAmount;
		}
		
	}
}

void AFloatingDataContainer::SetColor(FLinearColor InDamageColor)
{
	if (DamageNumberWidgetComponent)
	{
		UUserWidget* DamageNumberWidget = DamageNumberWidgetComponent->GetWidget();
		if (UFloatingDataWidget* DamageNumberWidgetCast = Cast<UFloatingDataWidget>(DamageNumberWidget))
		{
			DamageNumberWidgetCast->DamageColor = InDamageColor;
		}
		
	}
}

void AFloatingDataContainer::SetSize(float InDamageSize)
{
	if (DamageNumberWidgetComponent)
	{
		UUserWidget* DamageNumberWidget = DamageNumberWidgetComponent->GetWidget();
		if (UFloatingDataWidget* DamageNumberWidgetCast = Cast<UFloatingDataWidget>(DamageNumberWidget))
		{
			DamageNumberWidgetCast->DamageSize = InDamageSize;
		}
		
	}
}

void AFloatingDataContainer::HandleDestroy()
{
	if (IsValid(OwningActor))
	{
		OwningActor->ClearFloatingDamageRef();
	}
	Destroy();
}

// Called when the game starts or when spawned
void AFloatingDataContainer::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();

	//Directly over Spawn Location with Vertical Rise Offset.
	EndLocation = StartLocation + FVector(0, 0, VerticalRiseAmount);
	
	FTimerHandle DamageNumberTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DamageNumberTimerHandle, this, &AFloatingDataContainer::HandleDestroy, TimeUntilDestroy, false);
	
}

// Called every frame
void AFloatingDataContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;

	//Can be used Directly for Linear Interpolation.
	float Alpha = FMath::Clamp(ElapsedTime / TimeUntilDestroy, 0.0f, 1.0f);

	//Used for Bounce Effect.
	float BounceAlpha = FMath::InterpEaseOut(0.f, 1.f, Alpha, 4.0f);

	FVector StartLocationRef = StartLocation;
	
	float NewZLocation = FMath::Lerp(StartLocation.Z, EndLocation.Z, BounceAlpha);
	FVector FinalAdjustedLocation = FVector(StartLocationRef.X, StartLocationRef.Y, NewZLocation);
	SetActorLocation(FinalAdjustedLocation);
	
}

