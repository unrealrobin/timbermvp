﻿// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "TimberAiControllerBase.h"
#include "BossAIControllerBase.generated.h"

UCLASS()
class TIMBERMVP_API ABossAIControllerBase : public ATimberAiControllerBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossAIControllerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
