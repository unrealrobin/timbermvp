// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LeaderboardUserRankWidget.generated.h"

/**
 * 
 */
UCLASS()
class TIMBERMVP_API ULeaderboardUserRankWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Leaderboard")
	FString UserDisplayName = "Default";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Leaderboard")
	int32 UserRank = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Leaderboard")
	int64 HighestWaveCompleted = 0;
};
