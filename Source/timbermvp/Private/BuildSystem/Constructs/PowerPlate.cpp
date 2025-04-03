// Property of Paracosm Industries. Dont use my shit.


#include "BuildSystem/Constructs/PowerPlate.h"
#include "BuildSystem/BuildingComponents/TimberBuildingComponentBase.h"
#include "Character/TimberPlayableCharacter.h"
#include "Components/BoxComponent.h"


// Sets default values
APowerPlate::APowerPlate()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SnapCondition = ESnapCondition::FloorCenterSnapTopOnly;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionProfileName(TEXT("DR_BuildableBlockEverything"));
	CollisionBox = CreateDefaultSubobject<UBoxComponent>("CollisionBox");
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetCollisionProfileName(TEXT("DR_HitEventOnly"));
	
	
}

// Called when the game starts or when spawned
void APowerPlate::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APowerPlate::HitBoxBeginOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &APowerPlate::HitBoxEndOverlap);
	
}

void APowerPlate::FreeUpTrapSlotOnBuildingComponent()
{
	if (ATimberBuildingComponentBase* ParentBuildingComponent = Cast<ATimberBuildingComponentBase>(ParentBuildable))
	{
		ParentBuildingComponent->FrontCenterAttachment = nullptr;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find a stored Parent Building Component on PowerPlate. Trap Slot not freed."));
	}
}

void APowerPlate::HandleDeletionOfBuildable()
{
	FreeUpTrapSlotOnBuildingComponent();
	
	Super::HandleDeletionOfBuildable();
}

void APowerPlate::HandlePowerPlateMaterialChange(bool bShouldGlow)
{
	if (!StaticMeshComponent) return; // Safety check

	// Ensure we are working with a dynamic material instance
	UMaterialInstanceDynamic* MaterialInstance = Cast<UMaterialInstanceDynamic>(StaticMeshComponent->GetMaterial(1));

	if (!MaterialInstance)
	{
		UMaterialInterface* BaseMaterial = StaticMeshComponent->GetMaterial(1);
		if (!BaseMaterial) return;

		// Create and apply the dynamic instance
		MaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		StaticMeshComponent->SetMaterial(1, MaterialInstance);
	}

	// Set the emissive properties
	FVector Color = bShouldGlow ? FVector(1.0f, 3.0f, 5.0f) : FVector(5.0f, 2.0f, 1.0f);
	FLinearColor EmissiveColor = FLinearColor(Color.X, Color.Y, Color.Z, 1.0f);

	float EmissiveIntensity = bShouldGlow ? 3.0f : 0.1f;

	MaterialInstance->SetScalarParameterValue("EmissiveIntensity", EmissiveIntensity);
	MaterialInstance->SetVectorParameterValue("EmissiveColor", EmissiveColor);
}

void APowerPlate::HitBoxBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("PowerPlate HitBoxBeginOverlap"));

	ATimberPlayableCharacter* TimberCharacter = Cast<ATimberPlayableCharacter>(OtherActor);
	if (TimberCharacter)
	{
		//Adding 50% Damage Increase
		TimberCharacter->DamageModifierValue += .5f;
		//Power Plate Glow On
		HandlePowerPlateMaterialChange(true);

		//Gives the Character the PowerUp Overlay Material
		TimberCharacter->AddOverlayMaterialToCharacter(TimberCharacter->PowerUpOverlayMaterial, 0.0f);
	}
}

void APowerPlate::HitBoxEndOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("PowerPlate HitBoxEndOverlap"));

	ATimberPlayableCharacter* TimberCharacter = Cast<ATimberPlayableCharacter>(OtherActor);
	if (TimberCharacter)
	{
		//Removing 50% Damage Increase
		TimberCharacter->DamageModifierValue -= .5f;
		//Glow Off
		HandlePowerPlateMaterialChange(false);
		//Removes the PowerUp Overlay Material
		TimberCharacter->RemoveOverlayMaterialFromCharacter();
	}
}

// Called every frame
void APowerPlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

