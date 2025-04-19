// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LeaderboardWidget.generated.h"

class ULeaderboard;
class ULeaderboardUserRankWidget;
/**
 * 
 */
UCLASS()
class TIMBERMVP_API ULeaderboardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Leaderboard")
	TSubclassOf<ULeaderboardUserRankWidget> UserRankWidgetClass;

	UFUNCTION()
	void ProduceTopTenLeaderboardEntries();


	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* VerticalEntriesContainer;
	
private:
	UPROPERTY()
	ULeaderboard* LeaderboardSubsystem = nullptr;
};
