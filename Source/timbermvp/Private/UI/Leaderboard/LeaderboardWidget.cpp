﻿// Property of Paracosm.


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
		UE_LOG(LogTemp, Warning, TEXT("Leaderboard Widget - Bound to Leaderboard Subsystem."));
		LeaderboardSubsystem->OnSuccessfulLeaderboardQuery.AddDynamic(this, 
		&ULeaderboardWidget::ProduceTopTenLeaderboardEntries);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Leaderboard Widget - Failed to bind to Leaderboard Subsystem."));
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
					FString EntryDisplayName = LeaderboardSubsystem->GetLoginSubsystem()->GetDisplayNameFromAccountId(&Entry.AccountId);

					int32 Rank = Entry.Rank;

					int64 WavesCompleted = Entry.Score;


					//Create a LeaderboardUserRankWidget for Each Iteration.
					ULeaderboardUserRankWidget* UserRankWidget = CreateWidget<ULeaderboardUserRankWidget>(this, UserRankWidgetClass);
					if (UserRankWidget)
					{
						UE_LOG(LogTemp, Warning, TEXT("Successfully created UserRankWidget."));
						//Setting Variables on the newly created UserRankWidget
						UserRankWidget->UserDisplayName = EntryDisplayName;
						UserRankWidget->UserRank = Rank;
						UserRankWidget->HighestWaveCompleted = WavesCompleted;

						//Assigns the Actual Data to the Text Blocks in the Widget
						UserRankWidget->SetupLeaderBoardEntry();

						//Add the UserRankWidget to the UI
						if (VerticalEntriesContainer)
						{
							VerticalEntriesContainer->AddChildToVerticalBox(UserRankWidget);
							UE_LOG(LogTemp, Warning, TEXT("Added UserRankWidget to Vertical Box."));
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
