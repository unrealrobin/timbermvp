﻿// Property of Paracosm Industries. Dont use my shit.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/BuildableBase.h"
#include "ConstructBase.generated.h"

UCLASS()
class TIMBERMVP_API AConstructBase : public ABuildableBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AConstructBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
