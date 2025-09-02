// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MissionViewModel.generated.h"

/**
 * 
 */
UCLASS()
class TIMBERMVP_API UMissionViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	//UFUNCTION(BlueprintPure, FieldNotify)
	FString GetMissionTitle() const;

	//UFUNCTION(BlueprintPure, FieldNotify)
	FString GetMissionDescription() const;

	//UFUNCTION(BlueprintPure, FieldNotify)
	float GetProgressPercent() const;

	//UFUNCTION(BlueprintPure, FieldNotify)
	int GetProgressAmount() const;

	
protected:

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter)
	FString MissionTitle = "Test Mission Title";

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter)
	FString MissionDescription = "Mission Description Goes Here.";
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter)
	float ProgressPercent = 0.f;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter)
	int ProgressAmount = 0;
	
};
