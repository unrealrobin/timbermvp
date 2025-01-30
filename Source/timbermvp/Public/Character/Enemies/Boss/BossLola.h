// Property of Paracosm Industries. Don't use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemies/TimberEnemyCharacter.h"
#include "BossLola.generated.h"

UCLASS()
class TIMBERMVP_API ABossLola : public ATimberEnemyCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossLola();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCapsuleComponent* HeadCollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCapsuleComponent* LeftArmCollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCapsuleComponent* RightArmCollisionComponent;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UChildActorComponent* Drone1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UChildActorComponent* Drone2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UChildActorComponent* Drone3;

	//Handles the initial components of the boss.
	UFUNCTION(BlueprintCallable)
	void LolaInitializeComponents();
};
