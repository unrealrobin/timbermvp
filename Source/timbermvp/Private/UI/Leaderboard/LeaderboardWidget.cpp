// Property of Paracosm.


#include "UI/Leaderboard/LeaderboardWidget.h"

#include "Components/VerticalBox.h"
#include "Subsystems/Online/Leaderboard.h"
#include "Subsystems/Online/Login.h"
#include "UI/Leaderboard/LeaderboardUserRankWidget.h"

void ULeaderboardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	LeaderboardSubsystem = GetGameInstance()->GetSubsystem<ULeaderboard>();
	if (LeaderboardSubsystem)
	{
		LeaderboardSubsystem->OnSuccessfulLeaderboardQuery.AddDynamic(this, 
		&ULeaderboardWidget::ProduceTopTenLeaderboardEntries);
	}
}

void ULeaderboardWidget::ProduceTopTenLeaderboardEntries()
{
	ULogin* LoginSubsystem = GetGameInstance()->GetSubsystem<ULogin>();
	if (LoginSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Leaderboard Widget - LoginSubsystem is valid."));
		if (LeaderboardSubsystem)
		{
			UE_LOG(LogTemp, Warning, TEXT("Leaderboard Widget - Leaderboard Subsystem is valid."));
			if (LeaderboardSubsystem->TopTenEntries.Num() > 0)
			{
				for (FLeaderboardEntry& Entry : LeaderboardSubsystem->TopTenEntries)
				{
					UE_LOG(LogTemp, Warning, TEXT("Leaderboard Widget - Leaderboard Subsystem is valid."));
					
					FString EntryDisplayName = LeaderboardSubsystem->GetLoginSubsystem()->GetDisplayNameFromAccountId(&Entry.AccountId);

					int32 Rank = Entry.Rank;

					int64 WavesCompleted = Entry.Score;


					//Create a LeaderboardUserRankWidget for Each Iteration.
					ULeaderboardUserRankWidget* UserRankWidget = CreateWidget<ULeaderboardUserRankWidget>(this, UserRankWidget->StaticClass());
					if (UserRankWidget)
					{
						UserRankWidget->UserDisplayName = EntryDisplayName;
						UserRankWidget->UserRank = Rank;
						UserRankWidget->HighestWaveCompleted = WavesCompleted;

						//Add the UserRankWidget to the UI
						if (VerticalEntriesContainer)
						{
							VerticalEntriesContainer->AddChildToVerticalBox(UserRankWidget);
						}
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Failed to create UserRankWidget."));
					}
					
				}
			}
		}
	}
}
