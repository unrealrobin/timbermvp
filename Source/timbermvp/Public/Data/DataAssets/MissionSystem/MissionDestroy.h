// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "MissionBase.h"
#include "MissionDestroy.generated.h"

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UMissionDestroy : public UMissionBase
{
	GENERATED_BODY()
public:
	//If there is a Required Context Tag, Payload Context Tags must include all Tags in this Container.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
	FGameplayTagContainer RequiredContextTags;

	//Count here can be for example, DamageAmount or AmountOfEnemiesKilled
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
	int MissionCount = 0;

	
};
