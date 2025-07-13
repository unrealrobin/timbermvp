// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingDamageContainer.generated.h"

class UWidgetComponent;

UCLASS()
class TIMBERMVP_API AFloatingDamageContainer : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFloatingDamageContainer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene Component")
	USceneComponent* RootSceneComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Number")
	UWidgetComponent* DamageNumberWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Number")
	float TimeUntilDestroy = 1.0f;
	float ElapsedTime = 0.0f;
	
	void SetEffectText(FName EffectName);
	void SetDamageAmount(float InDamageAmount);
	
	void SetColor(FLinearColor InDamageColor);
	void SetSize(float InDamageSize);
	
protected:
	void HandleDestroy();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	FVector StartLocation = FVector::ZeroVector;
	
	FVector EndLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Number")
	float VerticalRiseAmount = 50.0f;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
