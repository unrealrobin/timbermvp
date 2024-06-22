// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Animation/AnimInstance.h"
#include "TimberAnimInstance.generated.h"


enum class ECharacterState : uint8;
class ATimberPlayableCharacter;
class ATimberPlayerController;

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UTimberAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	FInputActionValue InputActionValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Info")
	ATimberPlayableCharacter* OwningPawn;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Info")
	ATimberPlayerController* PlayerController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Info")
	bool CharacterNControllerInitialized = false;

	UFUNCTION(BlueprintCallable)
	void OnWeaponStateChange(EWeaponState WeaponState);
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool isRangedEquipped = false;

	UFUNCTION(BlueprintCallable)
	void InitializeDelegates();

private:
	
};
