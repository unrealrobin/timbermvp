// Property of Paracosm Industries. Dont use my shit.


#include "Weapons/TimberWeaponMeleeBase.h"

#include <string>

#include "Character/TimberCharacterBase.h"
#include "Character/TimberPlayableCharacter.h"
#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATimberWeaponMeleeBase::ATimberWeaponMeleeBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponBoxComponent = CreateDefaultSubobject<UBoxComponent>("Collision Box");
	WeaponBoxComponent->SetupAttachment(GetRootComponent());

	if(WeaponBoxComponent)
	{
	WeaponBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATimberWeaponMeleeBase::OnWeaponOverlapBegin);
	WeaponBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ATimberWeaponMeleeBase::OnWeaponOverlapEnd);
	}
}

// Called when the game starts or when spawned
void ATimberWeaponMeleeBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATimberWeaponMeleeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATimberWeaponMeleeBase::HandleWeaponCollision(bool ShouldReadyCollision) const
{
	if(ShouldReadyCollision)
	{
		GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Red, FString::Printf(TEXT("Sword Collision Enabled")));
		WeaponBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		WeaponBoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		WeaponBoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
		WeaponBoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);
	}
	else
	{
		WeaponBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ATimberWeaponMeleeBase::OnWeaponOverlapBegin(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	//Ignoring the Owning Character and the Actual Weapon itself.
	if(OtherActor == GetOwner() || OtherActor == this) return;

	GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Red, "Weapon Hit Something");


	/* If Melee Hit is Against the Player Character*/
	ATimberPlayableCharacter* HitCharacter = Cast<ATimberPlayableCharacter>(OtherActor);
	if(HitCharacter && !ActorsToIgnore.Contains(HitCharacter))
	{
		GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Red, FString::Printf(TEXT("Hit: %s"), *OtherActor->GetName()));
		ActorsToIgnore.Add(HitCharacter);
		//TODO:: When damage can be buffed, first calculate damage in some function then pass here.
		HitCharacter->PlayerTakeDamage(BaseWeaponDamage);
		return;
	}

	/*If Melee Hit is Against the AI Enemy Characters*/
	ATimberEnemyCharacter* HitEnemy = Cast<ATimberEnemyCharacter>(OtherActor);
	if(HitEnemy && !ActorsToIgnore.Contains(HitEnemy))
	{
		GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Red, FString::Printf(TEXT("Hit: %s"), *OtherActor->GetName()));
		HitEnemy->PlayMeleeWeaponHitSound(SweepResult);
		HitEnemy->TakeDamage(BaseWeaponDamage);
		ActorsToIgnore.Add(HitEnemy);
		
		/*IDamageableEnemy* DamageableEnemy = Cast<IDamageableEnemy>(HitEnemy);
		if(DamageableEnemy)
		{
			DamageableEnemy->PlayMeleeWeaponHitSound(SweepResult);
		}*/
	}
	
	
}

void ATimberWeaponMeleeBase::OnWeaponOverlapEnd(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Red, "Overlap Ended");

	//Resetting the Actors to Ignore Array for the next attack.
	ActorsToIgnore.Empty();
}

void ATimberWeaponMeleeBase::HandlePlayAttackMontage() const 
{

	const ATimberCharacterBase* Character = Cast<ATimberCharacterBase>(GetOwner());
	const int32 NumberOfMontageSections = AttackMontage->GetNumSections();
	
	const int32 RandomSection = UKismetMathLibrary::RandomIntegerInRange(1, NumberOfMontageSections);
	if(Character)
	{
		UAnimInstance* CharacterAnimInstance = Character->GetMesh()->GetAnimInstance();
		if(CharacterAnimInstance)
		{
			FName RandomSectionName = AttackMontage->GetSectionName(RandomSection-1);
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

