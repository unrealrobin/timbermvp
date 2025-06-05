// Property of Paracosm.


#include "UI/StatusEffects/StatusEffectBar.h"
#include "GameplayTagContainer.h"
#include "Components/HorizontalBox.h"
#include "UI/StatusEffects/StatusEffectBarIcon.h"


void UStatusEffectBar::NativeConstruct()
{
	Super::NativeConstruct();

	//AController* Controller = GetOwner()->GetController();
}

void UStatusEffectBar::AddEffectToBar(FGameplayTag EffectTag)
{
	//TODO:: Status Effect Component to Call this when adding a new Status Effect to Status Effect Container.

	UUserWidget* NewIcon = CreateWidget<UUserWidget>(GetWorld(), StatusEffectBarIconClass);
	if (UStatusEffectBarIcon* IconWidget = Cast<UStatusEffectBarIcon>(NewIcon))
	{
		//Checking the ID of the Effect Tag to determine the tint color for the icon.
		if (EffectTag == FGameplayTag::RequestGameplayTag("BuildableEffects.Id.Burn.Fire"))
		{
			IconWidget->StatusBarIconTint = FLinearColor((219.0f / 255.0f), (115.0f / 255.0), (76.0f / 255.0f), 1.0f); // Blue tint for Wet
		}
		else if (EffectTag == FGameplayTag::RequestGameplayTag("BuildableEffects.Id.Corrosive"))
		{
			IconWidget->StatusBarIconTint = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f); // Green tint for Corrosive
		}
		else if (EffectTag == FGameplayTag::RequestGameplayTag("BuildableEffects.Id.Slow.Frost"))
		{
			IconWidget->StatusBarIconTint = FLinearColor(0.0f, 0.0f, 1.0f, 1.0f); // Red tint for Burning
		}
		else
		{
			//For Status Effect attacks that shouldn't have an icon. These are Initial Damage only Effects.
			return;
		}

		//Setting the GameplayTag on the Widget - Used for Removal
		IconWidget->AssociatedEffectTag = EffectTag; // Set the associated effect tag
		
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

	if (StatusEffectIconContainer)
	{
		TArray<UWidget*> IconWidgetsArray = StatusEffectIconContainer->GetAllChildren();
		for (UWidget* Widget : IconWidgetsArray)
		{
			UStatusEffectBarIcon* IconWidget = Cast<UStatusEffectBarIcon>(Widget);
			if (IconWidget)
			{
				//Comparing the Linked Tag to the StatusBarIcon to the Effect Tag We Want to Remove.
				if (IconWidget->AssociatedEffectTag == EffectTag)
				{
					StatusEffectIconContainer->RemoveChild(IconWidget);
					break;
				}
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("Removed Effect: %s from Status Effect Bar"), *EffectTag.ToString());
	}
}
