// Property of Paracosm.


#include "Weapons/Abilities/RangedWeapon/KnockbackBlast.h"

#include "Components/BoxComponent.h"
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
		HitBox = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass());

		FRotator Rotation = OwningWeapon->ProjectileSpawnComponent->GetForwardVector().Rotation();
		Rotation.Yaw += 90.0f;
		HitBox->SetWorldRotation(Rotation);
		
		HitBox->SetWorldLocation(OwningWeapon->ProjectileSpawnComponent->GetComponentLocation());
		
		/*FVector Forward = OwningWeapon->ProjectileSpawnComponent->GetForwardVector();
		FRotator BlastRotation = Forward.Rotation();
		HitBox->SetWorldRotation(BlastRotation);*/

		HitBox->SetBoxExtent(InitialBoxExtent, true);
		
		HitBox->RegisterComponent();
		HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		HitBox->SetCollisionProfileName("DR_HitEventOnly");
		HitBox->OnComponentHit.AddDynamic(this, &UKnockbackBlast::HandleHit);
		
		InitialBoxLocation = HitBox->GetComponentLocation();
		
		GetWorld()->GetTimerManager().SetTimer(BlastTimerHandle, this, &UKnockbackBlast::IncrementBlastBox, 0.1f, true);
	}
}

void UKnockbackBlast::MoveBoxForward()
{
	float Delta = CurrentTimeStep / EndTimeStep;
	
	float ForwardOffset = FMath::Lerp(0.0f, EndLocationDelta, Delta );
	
	FVector AdjustedLocation = InitialBoxLocation + (HitBox->GetForwardVector() * ForwardOffset);

	HitBox->SetRelativeLocation(AdjustedLocation);
	
}

void UKnockbackBlast::ScaleBoxExtent()
{
	if (HitBox)
	{
		float Delta = CurrentTimeStep / EndTimeStep;
		FVector AdjustedVector = FMath::Lerp(InitialBoxExtent, EndBoxExtent, Delta);

		HitBox->SetBoxExtent(AdjustedVector, true);

		DrawDebugBox(GetWorld(), HitBox->GetComponentLocation(), AdjustedVector, FColor::Red, false, 0.1f, 0, 5);
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
	UE_LOG(LogTemp, Warning, TEXT("Incrementing Blast Box. CurrentTimeStep = %f"), CurrentTimeStep);
	MoveBoxForward();
	ScaleBoxExtent();
	UE_LOG(LogTemp, Warning, TEXT("Hit Box X Direction = %f"), HitBox->GetForwardVector().X);

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
