// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "TimberCharacterBase.h"
#include "TimberSeeda.generated.h"

UCLASS()
class TIMBERMVP_API ATimberSeeda : public ATimberCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATimberSeeda();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent;
};
