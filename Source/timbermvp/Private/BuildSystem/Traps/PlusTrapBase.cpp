// Property of Paracosm Industries. Dont use my shit.


#include "BuildSystem/Traps/PlusTrapBase.h"

#include "BuildSystem/BuildingComponents/TimberBuildingComponentBase.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
APlusTrapBase::APlusTrapBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxExtentRaycastStart = CreateDefaultSubobject<USceneComponent>("RaycastStart");
	BoxExtentRaycastStart->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlusTrapBase::BeginPlay()
{
	Super::BeginPlay();
}

void APlusTrapBase::RaycastForHitBoxLength()
{
	//Storing Raycast Hit Results Array Globally
	
	//Raycast
	FVector StartPoint = BoxExtentRaycastStart->GetComponentLocation();

	//Inversing the Right Vector to get the LeftVector (Needed because all building components are Y Forward)
	FVector EndPoint = StartPoint + ( -1 * BoxExtentRaycastStart->GetRightVector() * 800);
	
	bool bHits = GetWorld()->LineTraceMultiByChannel(HitResults, StartPoint, EndPoint, ECC_Visibility );

	//Storing Raycast Buildable Hit Location
	FVector HitLocation;
	if (bHits)
	{
		for (FHitResult Hit : HitResults)
		{
			// If Hit is a Buildable && Its not Itself && Its not the Hovered Component we are Attached the Trap to.
			 if (Cast<ABuildableBase>(Hit.GetActor())) //&& Cast<ATimberBuildingComponentBase>(Hit.GetActor()) != HoveredBuildingComponent && Hit.GetActor() != this
			 {
			 	UE_LOG(LogTemp, Warning, TEXT("Hit a Building Component"));
				HitLocation = Hit.ImpactPoint;
			 	//Debug the Hit Point
			 	DrawDebugSphere(GetWorld(), HitLocation, 10.f, 12, FColor::Blue, false, -1.f);
			 	DrawDebugSphere(GetWorld(), EndPoint, 10.f, 12, FColor::Red, false, -1.f);
			 	DrawDebugSphere(GetWorld(), StartPoint, 10.f, 12, FColor::Green, false, -1.f);
			 	DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, false, -1.f, 0, 2.f);
			 	

			 	
			 	break;
			 }
			 else
			 {
				 UE_LOG(LogTemp, Warning, TEXT("Hit a Non Building Component : %p"), Hit.GetActor());
			 }
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Hit"));
		DrawDebugSphere(GetWorld(), EndPoint, 10.f, 12, FColor::Red, false, -1.f);
		DrawDebugSphere(GetWorld(), StartPoint, 10.f, 12, FColor::Green, false, -1.f);
		DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, false, -1.f, 0, 2.f);
	}

	//The difference between the Raycast Start and End.
	float DistanceDifference = FVector::Dist(HitLocation, StartPoint);

	//The Value Clamped to not Exceed the Max Hit Box Length (2 Grid Squares for thise Trap)
	MaxHitBoxLength = UKismetMathLibrary::Clamp(DistanceDifference, 0.f, MaxHitBoxLength);

	//Setting the Box Extent Vector
	FVector DynamicBoxExtent = FVector(BoxWidth, MaxHitBoxLength, BoxHeight);

	//Setting the Box Extent
	HitBoxComponent->SetBoxExtent(DynamicBoxExtent);
	HitBoxComponent->SetRelativeLocation(FVector(0.f, MaxHitBoxLength, 0.f));

	FVector DebugBoxExtent = FVector(MaxHitBoxLength, BoxWidth, BoxHeight);
	//DrawDebugBox(GetWorld(), HitBoxComponent->GetComponentLocation(), DebugBoxExtent, FColor::Green, false, -1.f, 0, 2.f);
}

// Called every frame
void APlusTrapBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Need a way to stop this after the trap is placed.
	RaycastForHitBoxLength();
	
}

