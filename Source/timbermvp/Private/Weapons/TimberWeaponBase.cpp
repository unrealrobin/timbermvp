// Property of Paracosm Industries. Dont use my shit.


#include "Weapons/TimberWeaponBase.h"

#include "Character/TimberPlayableCharacter.h"
#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Components/BoxComponent.h"


// Sets default values
ATimberWeaponBase::ATimberWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponBoxComponent = CreateDefaultSubobject<UBoxComponent>("CapsuleComponent");
	RootComponent = WeaponBoxComponent;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(WeaponBoxComponent);

	//Used For Box Trace Multi Start & Stop
	TraceBoxStart = CreateDefaultSubobject<UBoxComponent>("TraceBoxStart");
	TraceBoxStart->SetupAttachment(RootComponent);
	TraceBoxEnd = CreateDefaultSubobject<UBoxComponent>("TraceBoxEnd");
	TraceBoxEnd->SetupAttachment(RootComponent);
	
}
// Called when the game starts or when spawned
void ATimberWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	//Collision Handeling
	WeaponBoxComponent->SetGenerateOverlapEvents(true);
	WeaponBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATimberWeaponBase::OnWeaponOverlapBegin);
	
}

void ATimberWeaponBase::HandlePlayAttackMontage() const
{
	
	// Getting Character and Character Anim Instance
	ATimberPlayableCharacter* TimberPlayableCharacter = Cast<ATimberPlayableCharacter>(GetOwner());
	if(!TimberPlayableCharacter) return;
	
	UAnimInstance* AnimInstance = TimberPlayableCharacter->GetMesh()->GetAnimInstance();
	if(!AnimInstance) return;
	
	
	//Creating an Array of Section Names and Selecting a Random Animation from Montage
	TArray<FName> StandardAttackSectionNames = {StandardAttack1, StandardAttack2};
	const int32 RandomAnim = FMath::RandRange(0, StandardAttackSectionNames.Num() - 1);
	
	AnimInstance->Montage_Play(AttackMontage, 1.f);
	AnimInstance->Montage_JumpToSection(StandardAttackSectionNames[RandomAnim], AttackMontage);
	
}

void ATimberWeaponBase::ReadyWeaponCollision(bool ShouldReadyCollision) const
{
	//TODO:: Implement this function to be called from Event Notifys in the Animations Montages.
	if(ShouldReadyCollision)
	{
		WeaponBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponBoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		WeaponBoxComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
		GEngine->AddOnScreenDebugMessage(1, 3.0, FColor::Green, "Weapon Collision Ready", false);
	}
	else
	{
		WeaponBoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		GEngine->AddOnScreenDebugMessage(2, 3.0, FColor::Green, "Weapon Collision Not Ready", false);
	}
}

void ATimberWeaponBase::OnWeaponOverlapBegin(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::Green, "Some Overlap Happened", false);
	PerformStandardAttack();
}


void ATimberWeaponBase::PerformStandardAttack()
{
	FVector StartTracePoint =  TraceBoxStart->GetComponentLocation();
	FVector EndTracePoint = TraceBoxEnd->GetComponentLocation();

	//Will store the hit result of all Hit Actors
	TArray<FHitResult> HitResults;

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(IgnoredActors);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = false;

	//Sweeping Against the Pawn Channel
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		StartTracePoint,
		EndTracePoint,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeBox(FVector(100, 100, 100)),
		QueryParams);

	//GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, "Performed Standard Attack");

	if(bHit)
	{
		
		for (const FHitResult& Hit : HitResults)
		{
			IDamageableEnemy* HitEnemy = Cast<IDamageableEnemy>(Hit.GetActor());
			if(HitEnemy)
			{
				HitEnemy->TakeDamage();
				GEngine->AddOnScreenDebugMessage(4, 3.0, FColor::Green, "Enemy Hit", false);
			}
			
		}
	}
	
}

