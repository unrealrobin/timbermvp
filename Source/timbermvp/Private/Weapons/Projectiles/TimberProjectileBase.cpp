// Property of Paracosm Industries. Dont use my shit.


#include "Weapons/Projectiles/TimberProjectileBase.h"

#include "Character/TimberPlayableCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interfaces/DamageableEnemy.h"

ATimberProjectileBase::ATimberProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	RootComponent = CapsuleComponent;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(RootComponent);
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement Component");

	
}

void ATimberProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	//Overlap Delegate
	if(CapsuleComponent)
	{
		CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ATimberProjectileBase::HandleOverlap);
	}
}

void ATimberProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATimberProjectileBase::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IDamageableEnemy* HitEnemy = Cast<IDamageableEnemy>(OtherActor);
	ATimberPlayableCharacter* TimberCharacter = Cast<ATimberPlayableCharacter>(GetOwner());

	

	FString DebugMessage = FString::Printf(TEXT("Hit Enemy: %s"), *OtherActor->GetName());
	GEngine->AddOnScreenDebugMessage(1, 4.0 , FColor::Green, DebugMessage);

	
	
	if(HitEnemy)
	{
		HitEnemy->TakeDamage(TimberCharacter->WeaponThreeInstance->GetWeaponBaseDamage());
		Destroy();
	}

	

	//TODO:: Ensure that the projectile is destroyed after hitting anything else. Make sure that the collision with self is ignored.
	Destroy();
}

