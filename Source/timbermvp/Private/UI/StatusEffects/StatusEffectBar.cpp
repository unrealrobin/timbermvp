// Property of Paracosm.


#include "UI/StatusEffects/StatusEffectBar.h"
#include "GameplayTagContainer.h"
#include "Components/HorizontalBox.h"
#include "UI/StatusEffects/StatusEffectBarIcon.h"


void UStatusEffectBar::AddEffectToBar(FGameplayTag EffectTag)
{
	//TODO:: Status Effect Component to Call this when adding a new Status Effect to Status Effect Container.

	UUserWidget* NewIcon = CreateWidget<UUserWidget>(GetWorld(), StatusEffectBarIconClass);
	if (UStatusEffectBarIcon* IconWidget = Cast<UStatusEffectBarIcon>(NewIcon))
	{
		//Checking the ID of the Effect Tag to determine the tint color for the icon.
		if (EffectTag == FGameplayTag::RequestGameplayTag("BuildableEffects.Id.Burn.Fire"))
		{
			IconWidget->StatusBarIconTint = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f); // Blue tint for Wet
		}
		else if (EffectTag == FGameplayTag::RequestGameplayTag("BuildableEffects.Id.Corrosive"))
		{
			IconWidget->StatusBarIconTint = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f); // Green tint for Corrosive
		}
		else if (EffectTag == FGameplayTag::RequestGameplayTag("BuildableEffects.Id.Slow.Frost"))
		{
			IconWidget->StatusBarIconTint = FLinearColor(0.0f, 0.0f, 1.0f, 1.0f); // Red tint for Burning
		}

		StatusEffectIconContainer->AddChild(IconWidget);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to cast NewIcon to UStatusEffectBarIcon"));
	}
}

void UStatusEffectBar::RemoveEffectFromBar(FGameplayTag EffectTag)
{
	//TODO:: Status Effect Component to Call this when removing an existing Status Effect from the Status Effect Container.
}
