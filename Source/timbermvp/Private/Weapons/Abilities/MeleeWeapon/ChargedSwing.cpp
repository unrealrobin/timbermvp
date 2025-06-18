// Property of Paracosm.


#include "Weapons/Abilities/MeleeWeapon/ChargedSwing.h"

#include "Character/TimberPlayableCharacter.h"
#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Components/SphereComponent.h"


UChargedSwing::UChargedSwing()
{
	ABILITY_NAME = "ChargedSwing";
	ValidationType = EAbilityValidation::RequiresPower;
	WeaponAbilityType = EForWeaponType::MeleeWeapon;
	bNeedsProjectileData = false;
	PowerCost = 30.0f;
}

void UChargedSwing::Execute(FAbilityContext Context)
{
	//Storing the Time since the Ability was pressed.
	ChargeTimer = GetWorld()->GetTimeSeconds();

	AbilityContext = Context;

	//Cannot Primary Ability Melee during animation
	Context.CombatComponent->bCanMeleeAttack = false;

	Context.CombatComponent->PlayCharacterAnimationMontage(ChargedSwingMontage, "WindUp", 1, true);
	CurrentMontageStage = EMontageStage::WindUp;
}

void UChargedSwing::Execute_Completed(FAbilityContext Context)
{
	Context.WeaponInstance->ConsumePower(PowerCost);

	//TODO:: Revisit if applying damage before of after the animation matters.
	
	Context.CombatComponent->PlayCharacterAnimationMontage(ChargedSwingMontage, "ChargedAttack", 1.0f);
	CurrentMontageStage = EMontageStage::Final;
	

}

void UChargedSwing::Execute_Cancelled(FAbilityContext Context)
{
	float CancelTime = GetWorld()->GetTimeSeconds();
	ChargeTimer = CancelTime - ChargeTimer;
	if (ChargeTimer > FullChargeTimer)
	{
		Execute_Completed(Context);
	}
	else
	{
		HandleIncompleteAbility(Context);
	}
}

void UChargedSwing::HandleIncompleteAbility(FAbilityContext Context)
{
	UE_LOG(LogTemp, Warning, TEXT("Player Released Before Full Charge Met."));
	//Stop Any Anim Montage
	if (Context.CombatComponent)
	{
		Context.CombatComponent->StopCharacterAnimationMontage(ChargedSwingMontage, .33f);
		//Cannot Primary Ability Melee during animation
		Context.CombatComponent->bCanMeleeAttack = true;
	}
	HandleCleanup(Context);
}

void UChargedSwing::HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	//Interrupted only true if the animation was interrupted.If finished properly, it will be false.
	//We only want to force a new montage if the previous montage was finished.
	//if (bInterrupted) return;

	if (CurrentMontageStage == EMontageStage::Final && !bInterrupted)
	{
		//Cannot Primary Ability Melee during animation
		AbilityContext.CombatComponent->bCanMeleeAttack = true;
		HandleCleanup(AbilityContext);
		UE_LOG(LogTemp, Warning, TEXT("Charged Swing Completed."));
	}
	
	
}

void UChargedSwing::HandleCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATimberEnemyCharacter* EnemyCharacter = Cast<ATimberEnemyCharacter>(OtherActor);
	if (ActorsToIgnore.Contains(OtherActor))
	{
		return;
	}
	
	if (EnemyCharacter && AbilityContext.Instigator)
	{
		//TODO:: Add Potential Stun Here with Timer. Make use of Status Effects?
		ActorsToIgnore.Add(EnemyCharacter);
		EnemyCharacter->TakeDamage(PerEnemyHitDamage, AbilityContext.Instigator);
	}
}

void UChargedSwing::HandleDamage(FAbilityContext Context)
{
	AActor* Weapon = Context.WeaponInstance;
	//UE_LOG(LogTemp, Warning, TEXT("Weapon : %s"), *Weapon->GetName());
	
	if (Weapon)
	{
		USphereComponent* CollisionSphere = NewObject<USphereComponent>(this);
		if (CollisionSphere)
		{
			CollisionSphere->SetupAttachment(Weapon->GetRootComponent());
			CollisionSphere->RegisterComponent();
			CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &UChargedSwing::HandleCollisionSphereBeginOverlap);
			CollisionSphere->SetSphereRadius(CollisionSphereRadius);
			CollisionSphere->SetCollisionProfileName("DR_HitEventOnly");
			CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

			//DrawDebugSphere(GetWorld(), Weapon->GetActorLocation(), CollisionSphereRadius, 12, FColor::Red, false, 10.0f);

			//Handles Destruction of the Collision Sphere and Emptys the Abilities ActorsToIgnore Array. (Clean up)
			FTimerHandle DestroyCollisionSphereTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(DestroyCollisionSphereTimerHandle, [this, CollisionSphere]()
			{
					if (CollisionSphere)
					{
						CollisionSphere->DestroyComponent();
						ActorsToIgnore.Empty();
					}
			}, .3f, false);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon Ability - HandleDamage() - Weapon is Null."));
	}
}
