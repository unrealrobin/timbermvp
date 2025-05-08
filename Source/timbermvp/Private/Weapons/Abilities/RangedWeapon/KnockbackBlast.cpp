// Property of Paracosm.


#include "Weapons/Abilities/RangedWeapon/KnockbackBlast.h"

#include "Character/Enemies/TimberEnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "UObject/FastReferenceCollector.h"
#include "Weapons/TimberWeaponRangedBase.h"

UKnockbackBlast::UKnockbackBlast()
{
	ABILITY_NAME = "KnockbackBlast";
	ValidationType = EAbilityValidation::RequiresPower;
	WeaponAbilityType = EForWeaponType::RangedWeapon;
	bNeedsProjectileData = false;
}

void UKnockbackBlast::Execute(FAbilityContext Context)
{
	//Spawn a UBoxComponent that starts at the muzzle of the Weapon.
	ATimberWeaponRangedBase* OwningWeapon = Cast<ATimberWeaponRangedBase>(Context.WeaponInstance);
	if (OwningWeapon && OwningWeapon->ProjectileSpawnComponent)
	{
		LocalContext = Context;
		
		HitBox = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass());

		FRotator Rotation = OwningWeapon->ProjectileSpawnComponent->GetComponentRotation();
		Rotation.Roll = 0.0f;
		FVector Location = OwningWeapon->ProjectileSpawnComponent->GetComponentLocation();
		
		HitBox->SetWorldRotation(Rotation);
		HitBox->SetWorldLocation(Location);
		HitBox->SetBoxExtent(InitialBoxExtent, true);

		/*For Debugging*/
		HitBox->SetHiddenInGame(false);
		HitBox->SetVisibility(true);

		HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		HitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
		HitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		
		
		HitBox->RegisterComponent();

		//HitBox->OnComponentHit.AddDynamic(this, &UKnockbackBlast::HandleHit);
		HitBox->OnComponentBeginOverlap.AddDynamic(this, &UKnockbackBlast::HandleOverlap);
		InitialBoxLocation = HitBox->GetComponentLocation();
	
		//UE_LOG(LogTemp, Warning, TEXT("PSC: Pitch: %f Yaw: %f Roll: %f"), Rotation.Pitch, Rotation.Yaw, Rotation.Roll);
		GetWorld()->GetTimerManager().SetTimer(BlastTimerHandle, this, &UKnockbackBlast::IncrementBlastBox, 0.05f, true);
		
		PlayNiagaraEffectAttached(NiagaraEffect, HitBox, NAME_None, EffectVectorOffset, FRotator::ZeroRotator, EAttachLocation::SnapToTargetIncludingScale, false, true);
		
		PlayEffectSFX(HitBox->GetComponentLocation(), SoundFXTrackName);
	}
}

void UKnockbackBlast::MoveBoxForward()
{
	float Delta = CurrentTimeStep / EndTimeStep;
	
	float ForwardOffset = FMath::Lerp(0.0f, EndLocationDelta, Delta );
	
	FVector AdjustedLocation = InitialBoxLocation + (HitBox->GetForwardVector() * ForwardOffset);

	FHitResult* Hit = nullptr;;
	HitBox->SetRelativeLocation(AdjustedLocation, true, Hit );
	
	
}

void UKnockbackBlast::ScaleBoxExtent()
{
	if (HitBox)
	{
		float Delta = CurrentTimeStep / EndTimeStep;
		FVector AdjustedVector = FMath::Lerp(InitialBoxExtent, EndBoxExtent, Delta);
		HitBox->SetBoxExtent(AdjustedVector, true);
	}
}

void UKnockbackBlast::HandleDestructionCheck()
{
	CurrentTimeStep++;
	
	if (CurrentTimeStep > EndTimeStep)
	{
		GetWorld()->GetTimerManager().ClearTimer(BlastTimerHandle);

		HitBox->DestroyComponent();
	}
}

void UKnockbackBlast::IncrementBlastBox()
{
	MoveBoxForward();
	ScaleBoxExtent();
	HandleDestructionCheck();
}

void UKnockbackBlast::HandleHit(
	UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("HitActor = %s"), *OtherActor->GetName());
	}
}

void UKnockbackBlast::HandleOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("HitActor = %s"), *OtherActor->GetName());
		if (ATimberEnemyCharacter* Enemy = Cast<ATimberEnemyCharacter>(OtherActor))
		{
			FVector ImpulseDirection = HitBox->GetForwardVector();
			ImpulseDirection.Z += 1.0f;
			ImpulseDirection = ImpulseDirection.GetSafeNormal();
			OtherComp->AddImpulse(ImpulseDirection * ImpulseForce);

			Enemy->TakeDamage(DamageAmount, LocalContext.Instigator);
			
		}
	}
}
