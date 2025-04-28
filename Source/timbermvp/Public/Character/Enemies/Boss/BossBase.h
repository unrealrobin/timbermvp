// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemies/TimberEnemyCharacter.h"
#include "BossBase.generated.h"

UCLASS()
class TIMBERMVP_API ABossBase : public ATimberEnemyCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossBase();

	/*Delegates*/
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDeath);
	FOnBossDeath OnBossDeath;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//virtual void HandleDropHealthLoot(TSubclassOf<AEnemyLootDropBase> HealthDropClass) override;

	UFUNCTION()
	void BroadcastDeathDelegate(AActor* DestroyedActor);

	
	
};
