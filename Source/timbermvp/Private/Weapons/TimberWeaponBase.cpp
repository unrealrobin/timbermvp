// Property of Paracosm Industries. Dont use my shit.


#include "Weapons/TimberWeaponBase.h"

#include "Character/TimberPlayableCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ATimberWeaponBase::ATimberWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	RootComponent = CapsuleComponent;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(CapsuleComponent);

}

// Called when the game starts or when spawned
void ATimberWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATimberWeaponBase::HandlePlayAttackMontage() const
{
	
	// Getting Character and Character Anim Instance
	ATimberPlayableCharacter* TimberPlayableCharacter = Cast<ATimberPlayableCharacter>(GetOwner());
	if(!TimberPlayableCharacter) return;
	
	UAnimInstance* AnimInstance = TimberPlayableCharacter->GetMesh()->GetAnimInstance();
	if(!AnimInstance) return;
	
	
	//Creating an Array of Section Names and Selecting a Random Animation from Montage
	TArray<FName> StandardAttackSectionNames = {StandardAttack1, StandardAttack2};
	const int32 RandomAnim = FMath::RandRange(0, StandardAttackSectionNames.Num() - 1);
	
	AnimInstance->Montage_Play(AttackMontage, 1.f);
	AnimInstance->Montage_JumpToSection(StandardAttackSectionNames[RandomAnim], AttackMontage);
	
	
}

