// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "Online/Leaderboards.h"
#include "Online/OnlineServices.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Leaderboard.generated.h"


class ULogin;
using namespace UE::Online;
/**
 * 
 */

DECLARE_LOG_CATEGORY_EXTERN(LogLeaderboard, Log, All);

UCLASS()
class TIMBERMVP_API ULeaderboard : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSuccessfulLeaderboardQuery);
	FSuccessfulLeaderboardQuery OnSuccessfulLeaderboardQuery;
	
	void HandleBindToWaveSubsytem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void InitializeOnlineServices();

	void QueryTopTenLeaderboard();
	
	TArray<FLeaderboardEntry> TopTenEntries;
	
	ULogin* GetLoginSubsystem();
private:
	IOnlineServicesPtr OnlineService = nullptr;
	ILeaderboardsPtr LeaderboardService = nullptr;
	IStatsPtr StatsService = nullptr;
	IAuthPtr LoginService = nullptr;

	
	void UpdateLocalOnlineUserStat(int CompletedWaveNumber);

	/*Wave Information*/
	UFUNCTION()
	void OnWaveComplete(int CompletedWaveNumber);
};
