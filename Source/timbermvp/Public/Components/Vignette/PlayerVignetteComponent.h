// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerVignetteComponent.generated.h"

UENUM(BlueprintType)
enum EDamageVignetteState
{
	FullHealth UMETA(DisplayName = "100%"),
	LightDamage UMETA(DisplayName = "80%"),
	HighDamage UMETA(DisplayName = "50%"),
	Critical UMETA(DisplayName = "25%"),
	Default UMETA(DisplayName = "Default"),
};

USTRUCT(BlueprintType)
struct FDamageVignetteState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageIntensity = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EdgeFalloff = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor VignetteColor = FLinearColor::Black;

	FDamageVignetteState() {}

	FDamageVignetteState(float InIntensity, float InFalloff, FLinearColor InColor)
		: DamageIntensity(InIntensity), EdgeFalloff(InFalloff), VignetteColor(InColor)
	{}
};



class UPostProcessComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TIMBERMVP_API UPlayerVignetteComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerVignetteComponent();

	//Where the Vignette gets added and modified.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UPostProcessComponent* VignettePostProcessComponent = nullptr;

	//Stores the Base Material used for the Vignette.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInterface* DamageVignetteMaterialBase = nullptr;

	//Not Assignable in Editor. Required for Adjusting Params at Runtime.
	UPROPERTY()
	UMaterialInstanceDynamic* DamageVignetteMid = nullptr;

	//Tracks the Current State of the Vignette.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vignette")
	EDamageVignetteState CurrentVignetteState = FullHealth;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Vignette State")
	FDamageVignetteState CurrentState = FDamageVignetteState(0.0f, 0.0f, FLinearColor::Red);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Vignette State")
	FDamageVignetteState FullState = FDamageVignetteState(0.0f, 0.0f, FLinearColor::Red);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Vignette State")
	FDamageVignetteState LightState = FDamageVignetteState(2.0f, 5.0f, FLinearColor::Red);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Vignette State")
	FDamageVignetteState HighState = FDamageVignetteState(4.0f, 4.0f, FLinearColor::Red);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Vignette State")
	FDamageVignetteState CriticalState = FDamageVignetteState(5.0f, 3.0f, FLinearColor::Red);

	FDamageVignetteState GetCurrentDamageVignetteState();
private:

	UFUNCTION()
	void InitializeVignetteDynamicMaterial();

	UFUNCTION()
	void HandleHealthChange(float HealthPercent);

	void LerpDamageVignetteState(FDamageVignetteState From, FDamageVignetteState To, float Time);
	
	
};
