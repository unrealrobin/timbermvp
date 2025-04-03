// Property of Paracosm Industries. Dont use my shit.


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
		WeaponBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATimberWeaponMeleeBase::OnAiWeaponOverlap);
		//WeaponBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ATimberWeaponMeleeBase::OnWeaponOverlapEnd);
		WeaponBoxComponent->OnComponentHit.AddDynamic(this, &ATimberWeaponMeleeBase::OnWeaponHit);
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
		UE_LOG(LogTemp, Warning, TEXT("TimberWeaponMeleeBase - HandleWeaponCollision() - Setting Collision Profile to HitEventOnly"));
		WeaponBoxComponent->SetCollisionProfileName("DR_HitEventOnly");
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TimberWeaponMeleeBase - HandleWeaponCollision() - Setting Collision Profile to NoCollision"));
		WeaponBoxComponent->SetCollisionProfileName("NoCollision");
	}
}

void ATimberWeaponMeleeBase::HandleAiSwordHit(AActor* Actor)
{
	UE_LOG(LogTemp, Warning, TEXT("Handling AI Sword Hit Response."));
	if (ActorsToIgnore.Contains(Actor))
	{
		return;
	}
	
	ActorsToIgnore.Add(Actor);
	UE_LOG(LogTemp, Warning, TEXT("Added Actor to Ignore Array: %s"), *Actor->GetName());
	
	ATimberBuildingComponentBase* HitBuildingComponent = Cast<ATimberBuildingComponentBase>(Actor);
	ATimberPlayableCharacter* HitCharacter = Cast<ATimberPlayableCharacter>(Actor);
	ATimberSeeda* Seeda = Cast<ATimberSeeda>(Actor);
	
	if (HitCharacter) //Is the player character and doesn't need to be ignored.
	{
		UE_LOG(LogTemp, Warning, TEXT("Applying Damage to Player. Amount = %f"), TotalWeaponDamage);
		HitCharacter->PlayerTakeDamage(TotalWeaponDamage);
		return;
	}
	if(Seeda)
	{
		Seeda->TakeDamage_Seeda(TotalWeaponDamage);
		return;
	}
	if (HitBuildingComponent && HitBuildingComponent->BuildingComponentType != EBuildingComponentType::Environment)
	{
		HitBuildingComponent->BuildingComponentTakeDamage(BaseWeaponDamage, GetOwner());
	}
}

void ATimberWeaponMeleeBase::HandlePlayerCharacterSwordHit(AActor* Actor)
{
	ATimberEnemyCharacter* HitEnemy = Cast<ATimberEnemyCharacter>(Actor);
	if (ActorsToIgnore.Contains(HitEnemy))
	{
		return;
	}

	HitEnemy->TakeDamage(TotalWeaponDamage, GetOwner());
}

void ATimberWeaponMeleeBase::OnWeaponHit(
	UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	const FHitResult& Hit)
{

	//Need a check to see if the weapon is being used by the player or the AI.
	ATimberPlayableCharacter* PlayerCharacter = Cast<ATimberPlayableCharacter>(GetOwner());
	ATimberEnemyCharacter* EnemyCharacter = Cast<ATimberEnemyCharacter>(GetOwner());

	if (EnemyCharacter)
	{
		HandleAiSwordHit(OtherActor);
		
	}
	else if (PlayerCharacter)
	{
		HandlePlayerCharacterSwordHit(OtherActor);
	}
	
	//First check if the actor is in the ActorsToIgnore Array.
	//If it is, return.
	
	//On a hit, add the Hit Actor to the enemies to ignore array.
	//Apply Damamge

	//When animation Ends, Empty the Array
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
		HandlePlayerCharacterSwordHit(OtherActor);
	}
}

/*void ATimberWeaponMeleeBase::OnWeaponOverlapBegin(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO:: Need to also configure this to work well with the Players Sword Overlapping, right now it will brake because of the CurrentTargetCheck
	//Ignoring the Owning Character, the Actual Weapon itself, and anything that is not the current target.
	if (OtherActor == GetOwner() || OtherActor == this || Cast<ATimberEnemyCharacter>(GetOwner())->CurrentTarget != 
	OtherActor)
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

	{/* Player using Sword - If Melee Hit is Against the AI Enemy Characters#1#
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
							BaseWeaponDamage * PlayerCharacter->DamageModifierValue );#1#
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
}*/

//Called in the Animations Montage using Notifies.
// Each Swing should result in only 1 dmg event per hit enemy. Resetting on Collision End Overlap can cause Double Hits,
// if the enemy has more than 1 collision box.
void ATimberWeaponMeleeBase::EmptyActorToIgnoreArray()
{
	//Resetting the Actors to Ignore Array for the next attack.
	ActorsToIgnore.Empty();
	if (ActorsToIgnore.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Emptying Actor to Ignore Array"));
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
