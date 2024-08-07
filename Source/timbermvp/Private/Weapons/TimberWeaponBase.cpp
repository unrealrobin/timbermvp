// Property of Paracosm Industries. Dont use my shit.


#include "Weapons/TimberWeaponBase.h"

#include "ActorReferencesUtils.h"
#include "Character/TimberPlayableCharacter.h"
#include "Character/Enemies/TimberEnemyCharacter.h"
#include"Weapons/Projectiles/TimberProjectileBase.h"
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

	ProjectileSpawnLocation = CreateDefaultSubobject<USceneComponent>("Projectile Spawn Location");
	ProjectileSpawnLocation->SetupAttachment(RootComponent);
	
}
// Called when the game starts or when spawned
void ATimberWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	//Collision Handeling
	WeaponBoxComponent->SetGenerateOverlapEvents(true);
	WeaponBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATimberWeaponBase::OnWeaponOverlapBegin);
	WeaponBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ATimberWeaponBase::OnWeaponOverlapEnd);

	
	
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
	if(ShouldReadyCollision)
	{
		WeaponBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponBoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		WeaponBoxComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);

		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(5, 2.0f, FColor::Green, "Weapon Collision Enabled.");
		}
		
	}
	else
	{
		WeaponBoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(5, 2.0f, FColor::Green, "Weapon Collision disabled.");
		}
	}
}

void ATimberWeaponBase::OnWeaponOverlapBegin(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	
	if(OtherActor == GetOwner() || OtherActor == this) return;
	if(ActorsToIgnore.Contains(OtherActor)) return;
	ApplyDamageOnOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex,
	bFromSweep, SweepResult);
}

void ATimberWeaponBase::OnWeaponOverlapEnd(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ActorsToIgnore.Empty();
}

void ATimberWeaponBase::ApplyDamageOnOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if(IDamageableEnemy* EnemyCharacter = Cast<IDamageableEnemy>(OtherActor))
	{
		EnemyCharacter->TakeDamage(BaseWeaponDamage);
	};
	
}

void ATimberWeaponBase::PerformStandardAttack()
{
	FVector StartTracePoint =  TraceBoxStart->GetComponentLocation();
	FVector EndTracePoint = TraceBoxEnd->GetComponentLocation();

	//Will store the hit result of all Hit Actors
	FHitResult HitResult;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(ActorsToIgnore);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = false;

	//Sweeping Against the Pawn Channel
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartTracePoint,
		EndTracePoint,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeBox(FVector(100, 100, 100)),
		QueryParams);

	
	if(bHit)
	{
			IDamageableEnemy* HitEnemy = Cast<IDamageableEnemy>(HitResult.GetActor());
			if(HitEnemy)
			{
				ActorsToIgnore.Add(Cast<AActor>(HitEnemy));
				DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 12, FColor::Green, false, 2.0f);
				HitEnemy->TakeDamage(BaseWeaponDamage);
			}
	}
	
}

void ATimberWeaponBase::FireRangedWeapon()
{
	ATimberPlayableCharacter* TimberCharacter = Cast<ATimberPlayableCharacter>(GetOwner());
	if(TimberCharacter && TimberCharacter->GetCurrentlyEquippedWeapon())
	{
		ATimberWeaponBase* CurrentWeapon = TimberCharacter->GetCurrentlyEquippedWeapon(); //Pistol

		if(ProjectileType)
		{
			FVector ProjectileSpawnLocationVector = CurrentWeapon->ProjectileSpawnLocation->GetComponentLocation();
			FRotator ControllerRotation = TimberCharacter->GetController()->GetControlRotation();

			if(GetWorld())
			{
				ATimberProjectileBase* SpawnedProjectile = GetWorld()->SpawnActor<ATimberProjectileBase>(ProjectileType, 
				ProjectileSpawnLocationVector, ControllerRotation);
				
				if(SpawnedProjectile)
				{
					SpawnedProjectile->SetOwner(TimberCharacter);
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(1, 4.0, FColor::Red, "Projectile Not Spawned");
				}
			}
		}
	}
}

