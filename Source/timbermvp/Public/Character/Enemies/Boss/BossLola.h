// Property of Paracosm Industries. Don't use my shit.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemies/TimberEnemyCharacter.h"
#include "BossLola.generated.h"

class ABossAIControllerBase;
class AFloaterDrones;

UENUM(BlueprintType)
enum class ELolaState : uint8
{
	Stunned UMETA(DisplayName = "Stunned"),
	NotStunned UMETA(DisplayName = "Not Stunned")
	
};


UCLASS()
class TIMBERMVP_API ABossLola : public ATimberEnemyCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossLola();
	void BindToDroneDeathDelegates();
	void InitializeLolaController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	ELolaState LolaState = ELolaState::NotStunned;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damage")
	bool bCanLolaTakeDamage = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drones")
	TArray<AFloaterDrones*> FloaterDronesArray;

	UFUNCTION()
	void PopulateDronesArray();

	UFUNCTION()
	void RandomizeDroneVulnerability();

	UFUNCTION()
	void RemoveDroneFromArray(AFloaterDrones* Drone);

	UFUNCTION()
	void SetDronesToNotDamageableDuringStun();

	FTimerHandle RandomizeDroneVulnerability_Handle;
	FTimerHandle LolaStunnedTimer_Handle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float LolaStunTime = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float RandomizationTime = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	ABossAIControllerBase* LolaController = nullptr;

public:

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	ELolaState GetLolaState() const;
	UFUNCTION()
	void SetLolaToNotStunned();
	UFUNCTION()
	void SetLolaToStunned(AFloaterDrones* Drone);

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCapsuleComponent* HeadCollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCapsuleComponent* LeftArmCollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCapsuleComponent* RightArmCollisionComponent;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UChildActorComponent* Drone1Component;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UChildActorComponent* Drone2Component;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UChildActorComponent* Drone3Component;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	AFloaterDrones* Drone1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	AFloaterDrones* Drone2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	AFloaterDrones* Drone3;

	

	//Handles the initial components of the boss.
	UFUNCTION(BlueprintCallable)
	void LolaInitializeComponents();
};




