// Property of Paracosm Industries. Dont use my shit.


#include "Weapons/TimberWeaponBase.h"

#include "ActorReferencesUtils.h"
#include "Character/TimberPlayableCharacter.h"
#include "Character/Enemies/TimberEnemyCharacter.h"
#include"Weapons/Projectiles/TimberProjectileBase.h"
#include "Components/BoxComponent.h"


// Sets default values
ATimberWeaponBase::ATimberWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = StaticMesh;
	
	ProjectileSpawnLocation = CreateDefaultSubobject<USceneComponent>("Projectile Spawn Location");
	ProjectileSpawnLocation->SetupAttachment(GetRootComponent());
	
}
// Called when the game starts or when spawned
void ATimberWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	//Collision Handeling
	/*
	WeaponBoxComponent->SetGenerateOverlapEvents(true);
	WeaponBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATimberWeaponBase::OnWeaponOverlapBegin);
	WeaponBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ATimberWeaponBase::OnWeaponOverlapEnd);
	*/

	
	
}

void ATimberWeaponBase::FireRangedWeapon()
{
	ATimberPlayableCharacter* TimberCharacter = Cast<ATimberPlayableCharacter>(GetOwner());
	if(TimberCharacter && TimberCharacter->GetCurrentlyEquippedWeapon())
	{
		ATimberWeaponBase* CurrentWeapon = TimberCharacter->GetCurrentlyEquippedWeapon(); //Pistol

		if(ProjectileType)
		{
			FVector ProjectileSpawnLocationVector = CurrentWeapon->ProjectileSpawnLocation->GetComponentLocation();
			FRotator ControllerRotation = TimberCharacter->GetController()->GetControlRotation();

			if(GetWorld())
			{
				ATimberProjectileBase* SpawnedProjectile = GetWorld()->SpawnActor<ATimberProjectileBase>(ProjectileType, 
				ProjectileSpawnLocationVector, ControllerRotation);
				
				if(SpawnedProjectile)
				{
					SpawnedProjectile->SetOwner(TimberCharacter);
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(1, 4.0, FColor::Red, "Projectile Not Spawned");
				}
			}
		}
	}
}

