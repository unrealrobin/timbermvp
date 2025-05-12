// Property of Paracosm.


#include "Weapons/Abilities/MeleeWeapon/ChargedSwing.h"

#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Weapons/TimberWeaponMeleeBase.h"

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

	Context.CombatComponent->PlayCharacterAnimationMontage(ChargedSwingMontage, "WindUp", .5, true);
	CurrentMontageStage = EMontageStage::WindUp;
}

void UChargedSwing::Execute_Completed(FAbilityContext Context)
{
	Context.WeaponInstance->ConsumePower(PowerCost);

	//TODO:: Revisit if applying damage before of after the animation matters.
	
	CurrentMontageStage = EMontageStage::Final;
	Context.CombatComponent->PlayCharacterAnimationMontage(ChargedSwingMontage, "ChargedAttack", 1.0f);
	
	//Sweep For Enemies and Apply Damage to Hit Actors.
	SweepForDamage(Context);

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

	if (CurrentMontageStage == EMontageStage::Final)
	{
		//Cannot Primary Ability Melee during animation
		AbilityContext.CombatComponent->bCanMeleeAttack = true;
		HandleCleanup(AbilityContext);
	}
	
}

void UChargedSwing::SweepForDamage(FAbilityContext Context)
{
	//Set Object Type to Sweep For.
	FCollisionObjectQueryParams DamageParams;
	DamageParams.AddObjectTypesToQuery(ECC_Pawn);
	

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(Context.Instigator);
	CollisionQueryParams.AddIgnoredActor(Context.WeaponInstance);
	
	//Temp Store for Hits
	TArray<FHitResult> HitResults;

	//Sweep Direction.
	FVector StartLocation = Context.Instigator->GetActorLocation();
	StartLocation.Z += 75.0f;
	
	FVector HitTargetLocation = Context.CombatComponent->GetProjectileTargetLocation();
	FVector EndLocation = StartLocation + ((HitTargetLocation - StartLocation).GetSafeNormal() * DamageDistance);
		//Sphere Sweep for Damage.
	
	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 10.0f);
	//DrawDebugSphere(GetWorld(), EndLocation, CollisionSphereRadius, 12, FColor::Red, false, 10.0f);

	bool bHit = GetWorld()->SweepMultiByObjectType(HitResults, StartLocation, EndLocation, FQuat::Identity, DamageParams, FCollisionShape::MakeSphere(CollisionSphereRadius), CollisionQueryParams);

	if (bHit)
	{
		for (FHitResult HitPawn: HitResults)
		{
			if (ATimberEnemyCharacter* HitEnemy = Cast<ATimberEnemyCharacter>(HitPawn.GetActor()))
			{
				
				HitEnemy->TakeDamage(PerEnemyHitDamage, Context.Instigator);
				if (NiagaraEffect)
				{
					PlayNiagaraEffectAtLocation(NiagaraEffect, HitPawn.ImpactPoint, FRotator::ZeroRotator);
				}
				UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitEnemy->GetName());
			}
		}
	}else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Hits from Sweep."));
	}
	
}
