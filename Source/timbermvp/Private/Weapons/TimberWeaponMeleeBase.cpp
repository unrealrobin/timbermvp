// Property of Paracosm Industries.


#include "Weapons/TimberWeaponMeleeBase.h"

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
	WeaponBoxComponent->SetCollisionProfileName("NoCollision");

	if (WeaponBoxComponent)
	{
		WeaponBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATimberWeaponMeleeBase::OnWeaponOverlapBegin);
		//WeaponBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ATimberWeaponMeleeBase::OnWeaponOverlapEnd);
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

//Alternates collision states, handled in Anim Notifies.
void ATimberWeaponMeleeBase::HandleWeaponCollision(bool ShouldReadyCollision) const
{
	if (ShouldReadyCollision)
	{
		UE_LOG(LogTemp, Warning, TEXT("TimberWeaponMeleeBase - HandleWeaponCollision() - Setting Collision Profile to HitEventOnly"));
		WeaponBoxComponent->SetCollisionProfileName("DR_HitEventOnly");
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TimberWeaponMeleeBase - HandleWeaponCollision() - Setting Collision Profile to NoCollision"));
		WeaponBoxComponent->SetCollisionProfileName("NoCollision");
	}
}

void ATimberWeaponMeleeBase::OnWeaponOverlapBegin(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	ATimberPlayableCharacter* PlayerCharacter = Cast<ATimberPlayableCharacter>(GetOwner());
	ATimberEnemyCharacter* EnemyCharacter = Cast<ATimberEnemyCharacter>(GetOwner());

	if (EnemyCharacter)
	{
		OnAiWeaponOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
	else if (PlayerCharacter)
	{
		OnPlayerWeaponOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}

void ATimberWeaponMeleeBase::OnWeaponOverlapEnd(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//We only run this for the player character, because we empty the actor array for AI enemies in the Animation Blueprint.
	if (Cast<ATimberPlayableCharacter>(GetOwner()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Character Emptying Actor to Ignore Array"));
		EmptyActorToIgnoreArray();
	}
}

//Runs Player Sword Overlap Logic
void ATimberWeaponMeleeBase::OnPlayerWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent*
		OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Player Weapon Overlapped with Actor: %s"), *OtherActor->GetName());
	ATimberEnemyCharacter* HitEnemy = Cast<ATimberEnemyCharacter>(OtherActor);
	if (HitEnemy)
	{
		if (ActorsToIgnore.Contains(HitEnemy))
		{
			return;
		}

		ActorsToIgnore.Add(HitEnemy);
		
		HitEnemy->TakeDamage(TotalWeaponDamage, GetOwner());
	}
}

void ATimberWeaponMeleeBase::OnAiWeaponOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Handling AI Sword Hit Response."));
	if (ActorsToIgnore.Contains(OtherActor) ||
		OtherActor != Cast<ATimberEnemyCharacter>(GetOwner())->CurrentTarget ||
		OtherActor == GetOwner() || OtherActor == this)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Ignoring Actor: %s"), *OtherActor->GetName());
		return;
	}
	
	ActorsToIgnore.Add(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("Added Actor to Ignore Array: %s"), *OtherActor->GetName());
	
	ATimberBuildingComponentBase* HitBuildingComponent = Cast<ATimberBuildingComponentBase>(OtherActor);
	ATimberPlayableCharacter* HitCharacter = Cast<ATimberPlayableCharacter>(OtherActor);
	ATimberSeeda* Seeda = Cast<ATimberSeeda>(OtherActor);
	
	if (HitCharacter) //Is the player character and doesn't need to be ignored.
	{
		UE_LOG(LogTemp, Warning, TEXT("Applying Damage to Player. Amount = %f"), TotalWeaponDamage);
		HitCharacter->PlayerTakeDamage(TotalWeaponDamage);
		return;
	}
	if(Seeda)
	{
		UE_LOG(LogTemp, Warning, TEXT("Applying Damage to Seeda. Amount = %f"), TotalWeaponDamage);
		Seeda->TakeDamage_Seeda(TotalWeaponDamage);
		return;
	}
	if (HitBuildingComponent && HitBuildingComponent->BuildingComponentType != EBuildingComponentType::Environment)
	{
		HitBuildingComponent->BuildingComponentTakeDamage(BaseWeaponDamage, GetOwner());
	}
}

void ATimberWeaponMeleeBase::EmptyActorToIgnoreArray()
{
	ActorsToIgnore.Empty();
}

void ATimberWeaponMeleeBase::HandleAttackMontageInterrupted(UAnimMontage* AnimMontage, bool bArg, bool bCond)
{
	//TODO:: Move CanAttackAgain to CombatComponent - used in validation of Melee Weapons
	
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
	const int32 NumberOfMontageSections = PrimaryAbilityMontage->GetNumSections();

	//Getting Random Section of Montage
	const int32 RandomSection = UKismetMathLibrary::RandomIntegerInRange(1, NumberOfMontageSections);
	if (Character)
	{
		UAnimInstance* CharacterAnimInstance = Character->GetMesh()->GetAnimInstance();
		if (CharacterAnimInstance)
		{
			FName RandomSectionName = PrimaryAbilityMontage->GetSectionName(RandomSection - 1);
			
			if (!BlendingOutDelegate.IsBound())
			{
				//Creating a delegate to the BlendingOut function to reset the CanAttackAgain bool on the Player Controller.
				//For any reason of blending out, we revert the CanAttackAgain bool to true.
				//Either Animation Completed or interrupted.
				BlendingOutDelegate.BindUObject(this, &ATimberWeaponMeleeBase::HandleAttackMontageInterrupted, false);
				CharacterAnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, PrimaryAbilityMontage);
			}
			CharacterAnimInstance->Montage_Play(PrimaryAbilityMontage, 1.f);
			CharacterAnimInstance->Montage_JumpToSection(RandomSectionName);
		}
	}
}

/*void ATimberWeaponMeleeBase::PerformStandardAttack()
{
	HandlePlayAttackMontage();
}*/


