// Property of Paracosm Industries. Dont use my shit.


#include "Weapons/TimberWeaponMeleeBase.h"

#include <string>

#include "BuildSystem/BuildingComponents/TimberBuildingComponentBase.h"
#include "Character/TimberCharacterBase.h"
#include "Character/TimberPlayableCharacter.h"
#include "Character/TimberSeeda.h"
#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "Controller/TimberPlayerController.h"
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
		//GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Red, FString::Printf(TEXT("Sword Collision Enabled")));
		WeaponBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		WeaponBoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		WeaponBoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	}
	else
	{
		WeaponBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponBoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		WeaponBoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	}
}

void ATimberWeaponMeleeBase::OnWeaponOverlapBegin(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	//Ignoring the Owning Character  and the Actual Weapon itself.
	if (OtherActor == GetOwner() || OtherActor == this)
	{
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Sword Overlapped Component: %s. Sword Overlapped Actor: %s"), *OverlappedComponent->GetName(), *OtherActor->GetName());
	//UE_LOG(LogTemp, Warning, TEXT("Sword Weapon Owner: %s"), *GetOwner()->GetName());
	
	{ // AI using sword - Ignoring other AI
		if (Cast<ATimberEnemyCharacter>(GetOwner()))
		{
			ATimberBuildingComponentBase* HitBuildingComponent = Cast<ATimberBuildingComponentBase>(OtherActor);
			ATimberPlayableCharacter* HitCharacter = Cast<ATimberPlayableCharacter>(OtherActor);
			ATimberSeeda* Seeda = Cast<ATimberSeeda>(OtherActor);
			if (ActorsToIgnore.Contains(HitCharacter) || ActorsToIgnore.Contains(Seeda))
			{
				return;
			}
			if (HitCharacter) //Is the player character and doesn't need to be ignored.
			{
				ActorsToIgnore.Add(HitCharacter);
				
				HitCharacter->PlayerTakeDamage(TotalWeaponDamage);
				return;
			}
			if(Seeda)
			{
				ActorsToIgnore.Add(Seeda);
				Seeda->TakeDamage_Seeda(TotalWeaponDamage);
				//UE_LOG(LogTemp, Warning, TEXT("Melee Weapon - Seeda:Total Weapon Damage: %f"), TotalWeaponDamage);
				return;
			}
			if (HitBuildingComponent && HitBuildingComponent->BuildingComponentType != EBuildingComponentType::Environment)
			{
				HitBuildingComponent->BuildingComponentTakeDamage(BaseWeaponDamage, GetOwner());
				//UE_LOG(LogTemp, Warning, TEXT("Melee Weapon - Building Component: Base Weapon Damage: %f"), BaseWeaponDamage);
			}
		}
	}

	{/* Player using Sword - If Melee Hit is Against the AI Enemy Characters*/
		if (Cast<ATimberPlayableCharacter>(GetOwner()))
		{
			ATimberEnemyCharacter* HitEnemy = Cast<ATimberEnemyCharacter>(OtherActor);
			if (ActorsToIgnore.Contains(HitEnemy))
			{
				return;
			}
			if (HitEnemy)
			{
				IDamageableEnemy* DamageableEnemy = Cast<IDamageableEnemy>(OtherActor);
				if (DamageableEnemy)
				{
					DamageableEnemy->PlayMeleeWeaponHitSound(SweepResult);
					ATimberPlayableCharacter* PlayerCharacter = Cast<ATimberPlayableCharacter>(GetOwner());
					// Print the owner of the weapon
					//UE_LOG(LogTemp, Warning, TEXT("Sword Weapon Owner: %s"), *GetOwner()->GetName());

					if (PlayerCharacter)
					{
						DamageableEnemy->TakeDamage(BaseWeaponDamage * PlayerCharacter->DamageModifierValue, GetOwner());
						/*UE_LOG(LogTemp, Warning, TEXT("Sword Base Damage: %f. Player Character Damage Modifier: %f. TotalDamage: %f. "),
							BaseWeaponDamage,
							PlayerCharacter->DamageModifierValue,
							BaseWeaponDamage * PlayerCharacter->DamageModifierValue );*/
					}
				}
				ActorsToIgnore.Add(HitEnemy);
			}
		}
	}
}

void ATimberWeaponMeleeBase::OnWeaponOverlapEnd(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Red, "Overlap Ended");

	//Will also be forced at the end of the attack animation through Notifies.
	EmptyActorToIgnoreArray();
}

//Called in the Animations Montage using Notifies.
// Each Swing should result in only 1 dmg event per hit enemy. Resetting on Collision End Overlap can cause Double Hits,
// if the enemy has more than 1 collision box.
void ATimberWeaponMeleeBase::EmptyActorToIgnoreArray()
{
	//Resetting the Actors to Ignore Array for the next attack.
	ActorsToIgnore.Empty();
}

void ATimberWeaponMeleeBase::HandleAttackMontageInterrupted(UAnimMontage* AnimMontage, bool bArg, bool bCond)
{
	//Get the Controller and Flip the bCanAttackAgain Bool to True.
	ATimberPlayerController* PlayerController = Cast<ATimberPlayerController>(Cast<ATimberPlayableCharacter>(GetOwner())->GetController());
	if (PlayerController)
	{
		PlayerController->CanAttackAgain = true;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("TimberWeaponMeleeBase - HandleAttackMontageInterrupted() - Player Controller is NULL"));
	}
}

void ATimberWeaponMeleeBase::HandlePlayAttackMontage()
{
	const ATimberCharacterBase* Character = Cast<ATimberCharacterBase>(GetOwner());
	const int32 NumberOfMontageSections = AttackMontage->GetNumSections();

	//Getting Random Section of Montage
	const int32 RandomSection = UKismetMathLibrary::RandomIntegerInRange(1, NumberOfMontageSections);
	if (Character)
	{
		UAnimInstance* CharacterAnimInstance = Character->GetMesh()->GetAnimInstance();
		if (CharacterAnimInstance)
		{
			FName RandomSectionName = AttackMontage->GetSectionName(RandomSection - 1);

			//Checking binding so we dont double bind the delegate on subsequence attacks.
			if (!BlendingOutDelegate.IsBound())
			{
				BlendingOutDelegate.BindUObject(this, &ATimberWeaponMeleeBase::HandleAttackMontageInterrupted, false);
				CharacterAnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, AttackMontage);
			}
			CharacterAnimInstance->Montage_Play(AttackMontage, 1.f);
			CharacterAnimInstance->Montage_JumpToSection(RandomSectionName);
		}
	}
}

void ATimberWeaponMeleeBase::PerformStandardAttack()
{
	HandlePlayAttackMontage();
}
