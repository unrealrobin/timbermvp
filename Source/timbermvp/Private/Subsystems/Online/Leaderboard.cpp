// Property of Paracosm.


#include "Subsystems/Online/Leaderboard.h"
#include "Online/OnlineAsyncOpHandle.h"
#include "Online/OnlineResult.h"
#include "Online/Stats.h"
#include "Subsystems/Online/Login.h"
#include "Subsystems/Wave/WaveGameInstanceSubsystem.h"

DEFINE_LOG_CATEGORY(LogLeaderboard)

void ULeaderboard::HandleBindToWaveSubsytem()
{
	//Gets called from the WaveGameInstanceSubsystem after initialization to ensure we can bind to OnWaveComplete Delegate.
	if (UWaveGameInstanceSubsystem* WaveSubsystem = GetGameInstance()->GetSubsystem<UWaveGameInstanceSubsystem>())
	{
		WaveSubsystem->HandleWaveComplete.AddDynamic(this, &ULeaderboard::OnWaveComplete);
		UE_LOG(LogLeaderboard, Warning, TEXT("Bound to Wave Subsystem. WORKING."));
	}
	else
	{
		UE_LOG(LogLeaderboard, Warning, TEXT("Not Bound to Wave Subsystem."));
		
	}
}

void ULeaderboard::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	InitializeOnlineServices();
	UE_LOG(LogLeaderboard, Warning, TEXT("Leaderboard Subsystem Initialized."));
}

ULogin* ULeaderboard::GetLoginSubsystem()
{
	if (ULogin* LoginSubsystem = GetGameInstance()->GetSubsystem<ULogin>())
	{
		return LoginSubsystem;
	}

	return nullptr;
}

void ULeaderboard::OnWaveComplete(int CompletedWaveNumber)
{
	ULogin* Login = GetLoginSubsystem();
	if (StatsService.IsValid() && Login)
	{
		//Get Local Account ID - Available on Login Struct
		FAccountId PlayerAccountId = Login->UserInfo.AccountInfo.AccountId;;
		
		//Configuring the StatValue
		int64 WaveNumberAsINT64 = static_cast<int64>(CompletedWaveNumber);
		FStatValue StatValue;
		StatValue.VariantType = ESchemaAttributeType::Int64;
		StatValue.Set(WaveNumberAsINT64);

		//Setting Which stat to Update with which value.
		FUserStats UserStats;
		UserStats.AccountId = PlayerAccountId;
		UserStats.Stats.Add(TEXT("HIGHEST_COMPLETED_WAVE"), StatValue);
		
		//Utilizing the UpdateStats function to send the data to the backend.
		FUpdateStats::Params Params;
		Params.LocalAccountId = PlayerAccountId;
		Params.UpdateUsersStats.Add(UserStats);
		StatsService->UpdateStats(MoveTemp(Params)).OnComplete([](const TOnlineResult<FUpdateStats>& Result)
		{
			if (Result.IsOk())
			{
				if (GEngine)
				{
					UE_LOG(LogLeaderboard, Warning, TEXT("Sent Updated Stats to Stat Service. Result is OK."));
				}
			}
			else
			{
				UE_LOG(LogLeaderboard, Warning, TEXT("Sent Updated Stats to Stat Service. Result is NOT OK."));
			}
		});
	}
}

void ULeaderboard::InitializeOnlineServices()
{
	OnlineService = GetServices();
	if (OnlineService.IsValid())
	{
		LeaderboardService = OnlineService->GetLeaderboardsInterface();
		StatsService = OnlineService->GetStatsInterface();
		if (LeaderboardService.IsValid() && StatsService.IsValid())
		{
			
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Leaderboard or Stats service is not valid."));
		}
	}
}

