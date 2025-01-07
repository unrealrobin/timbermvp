// Property of Paracosm Industries. Dont use my shit.


#include "Weapons/TimberWeaponMeleeBase.h"

#include <string>

#include "Character/TimberCharacterBase.h"
#include "Character/TimberPlayableCharacter.h"
#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default value
ATimberWeaponMeleeBase::ATimberWeaponMeleeBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponBoxComponent = CreateDefaultSubobject<UBoxComponent>("Collision Box");
	WeaponBoxComponent->SetupAttachment(GetRootComponent());
	WeaponBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WeaponBoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

	if (WeaponBoxComponent)
	{
		WeaponBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATimberWeaponMeleeBase::OnWeaponOverlapBegin);
		WeaponBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ATimberWeaponMeleeBase::OnWeaponOverlapEnd);
	}
}

// Called when the game starts or when spawned
void ATimberWeaponMeleeBase::BeginPlay()
{
	Super::BeginPlay();

	WeaponOwner = GetOwner();
	WeaponInstigator = Cast<ATimberPlayableCharacter>(GetInstigator());
}

// Called every frame
void ATimberWeaponMeleeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATimberWeaponMeleeBase::HandleWeaponCollision(bool ShouldReadyCollision) const
{
	if (ShouldReadyCollision)
	{
		GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Red, FString::Printf(TEXT("Sword Collision Enabled")));
		WeaponBoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		WeaponBoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	}
	else
	{
		WeaponBoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		WeaponBoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	}
}

void ATimberWeaponMeleeBase::OnWeaponOverlapBegin(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	//Ignoring the Owning Character (Instigator) and the Actual Weapon itself.
	if (OtherActor == WeaponInstigator || OtherActor == this)
	{
		return;
	}
	/* If Melee Hit is Against the Player Character
	Used for when the AI Enemy have a Melee Weapon
	Ensures swings only hit Player Characters and not other AI Enemy
	*/
	ATimberPlayableCharacter* HitCharacter = Cast<ATimberPlayableCharacter>(OtherActor);
	if (ActorsToIgnore.Contains(HitCharacter))
	{
		return;
	}
	if (HitCharacter)
	{
		GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Red, FString::Printf(TEXT("Hit: %s"), *OtherActor->GetName()));
		ActorsToIgnore.Add(HitCharacter);
		//TODO:: When damage can be buffed, first calculate damage in some function then pass here.
		HitCharacter->PlayerTakeDamage(BaseWeaponDamage);
		return;
	}

	/*If Melee Hit is Against the AI Enemy Characters*/
	ATimberEnemyCharacter* HitEnemy = Cast<ATimberEnemyCharacter>(OtherActor);
	if (ActorsToIgnore.Contains(HitEnemy))
	{
		return;
	}
	if (HitEnemy && !ActorsToIgnore.Contains(HitEnemy))
	{
		IDamageableEnemy* DamageableEnemy = Cast<IDamageableEnemy>(OtherActor);
		if (DamageableEnemy)
		{
			DamageableEnemy->PlayMeleeWeaponHitSound(SweepResult);
			DamageableEnemy->TakeDamage(BaseWeaponDamage);
		}
		ActorsToIgnore.Add(HitEnemy);
	}
}

void ATimberWeaponMeleeBase::OnWeaponOverlapEnd(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Red, "Overlap Ended");
}

//Called in the Animations Montage using Notifies.
// Each Swing should result in only 1 dmg event per hit enemy. Resetting on Collision End Overlap can cause Double Hits,
// if the enemy has more than 1 collision box.
void ATimberWeaponMeleeBase::EmptyActorToIgnoreArray()
{
	//Resetting the Actors to Ignore Array for the next attack.
	ActorsToIgnore.Empty();
}

void ATimberWeaponMeleeBase::HandlePlayAttackMontage() const
{
	const ATimberCharacterBase* Character = Cast<ATimberCharacterBase>(WeaponInstigator);
	const int32 NumberOfMontageSections = AttackMontage->GetNumSections();

	const int32 RandomSection = UKismetMathLibrary::RandomIntegerInRange(1, NumberOfMontageSections);
	if (Character)
	{
		UAnimInstance* CharacterAnimInstance = Character->GetMesh()->GetAnimInstance();
		if (CharacterAnimInstance)
		{
			FName RandomSectionName = AttackMontage->GetSectionName(RandomSection - 1);
			UE_LOG(LogTemp, Warning, TEXT("Section Name: %s"), *RandomSectionName.ToString());
			UE_LOG(LogTemp, Warning, TEXT("Section Number: %i"), RandomSection);
			CharacterAnimInstance->Montage_Play(AttackMontage, 1.f);
			CharacterAnimInstance->Montage_JumpToSection(RandomSectionName);
		}
	}
}

void ATimberWeaponMeleeBase::PerformStandardAttack()
{
	HandlePlayAttackMontage();
}
