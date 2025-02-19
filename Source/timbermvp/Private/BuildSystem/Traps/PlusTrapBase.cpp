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

	//Inverse the Right Vector to get the LeftVector (Needed because all building components are Y Forward)
	//FVector EndPoint = StartPoint + ( -1 * BoxExtentRaycastStart->GetRightVector() * MaxHitBoxLength);
	FVector EndPoint = StartPoint + ( BoxExtentRaycastStart->GetRightVector() * MaxHitBoxLength);
	
	bool bHits = GetWorld()->LineTraceMultiByChannel(HitResults, StartPoint, EndPoint, ECC_Visibility );

	
	//We default this to EndPoint for in the case there is no hit, The Max Distance we want the HitBox to be is the MaxHitBoxLength
	FVector HitLocation = EndPoint;
	
	if (bHits)
	{
		for (FHitResult Hit : HitResults)
		{
			// If Hit is a Buildable && It's not Itself 
			 if (Cast<ABuildableBase>(Hit.GetActor()) && Cast<ATimberBuildingComponentBase>(Hit.GetActor()) != HoveredBuildingComponent && Hit.GetActor() != this) 
			 {
			 	UE_LOG(LogTemp, Warning, TEXT("Hit a Building Component"));
				HitLocation = Hit.ImpactPoint;
			 	//Debug the Hit Point
			 	/*DrawDebugSphere(GetWorld(), HitLocation, 10.f, 12, FColor::Blue, false, -1.f);
			 	DrawDebugSphere(GetWorld(), EndPoint, 10.f, 12, FColor::Red, false, -1.f);
			 	DrawDebugSphere(GetWorld(), StartPoint, 10.f, 12, FColor::Green, false, -1.f);
			 	DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, false, -1.f, 0, 2.f);*/
			 	break;
			 }
			 else
			 {
				 UE_LOG(LogTemp, Warning, TEXT("Hit a Non Building Component : %p"), *Hit.GetActor()->GetName());
			 }
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("No Hit"));
		/*DrawDebugSphere(GetWorld(), HitLocation, 10.f, 12, FColor::Blue, false, -1.f);
		DrawDebugSphere(GetWorld(), EndPoint, 10.f, 12, FColor::Red, false, -1.f);
		DrawDebugSphere(GetWorld(), StartPoint, 10.f, 12, FColor::Green, false, -1.f);
		DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, false, -1.f, 0, 2.f);*/
	}

	//The difference between the Raycast Start and End.
	float DistanceDifference = FVector::Dist(HitLocation, StartPoint);

	//The Value Clamped to not Exceed the Max Hit Box Length (2 Grid Squares for thise Trap)
	MaxHitBoxLength = UKismetMathLibrary::Clamp(DistanceDifference, 1.f, MaxHitBoxLength);

	BoxLength = MaxHitBoxLength / 2 + 30.f;

	//Setting the Box Extent Vector
	FVector DynamicBoxExtent = FVector(BoxWidth, BoxLength, BoxHeight);

	//Setting the Box Extent
	HitBoxComponent->SetBoxExtent(DynamicBoxExtent);

	HitBoxComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

	//Relative Locations because when scaling extents the box components scales in 2 directions
	// -y <--MHBL--> +y
	//So we position the center of the Dynamic HitBox in the middle of the Extent Scale
	// So if we Want to scale the box to be 800 units long, we need to set the relative location to be 400 units in the Y direction (400 In each Direction)
	// We use 30 Units to account for the Offset of the Raycast Start point from the Actual Trap Center (+- 30/40)
	HitBoxComponent->SetRelativeLocation(FVector(0.f, BoxLength, 0.f));
}

// Called every frame
void APlusTrapBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Need a way to stop this after the trap is placed.
	RaycastForHitBoxLength();
	
}

