// Property of Paracosm.


#include "UI/StatusEffects/StatusEffectBar.h"
#include "GameplayTagContainer.h"
#include "Components/HorizontalBox.h"
#include "UI/StatusEffects/StatusEffectBarIcon.h"


void UStatusEffectBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UStatusEffectBar::AddEffectToBar(FGameplayTag EffectTag)
{
	UUserWidget* NewIcon = CreateWidget<UUserWidget>(GetWorld(), StatusEffectBarIconClass);
	if (UStatusEffectBarIcon* IconWidget = Cast<UStatusEffectBarIcon>(NewIcon))
	{
		//Checking the ID of the Effect Tag to determine the tint color for the icon.
		if (EffectTag == FGameplayTag::RequestGameplayTag("SynergySystem.Effect.Fire.Burn"))
		{
			IconWidget->StatusBarIconTint = FLinearColor((219.0f / 255.0f), (115.0f / 255.0), (76.0f / 255.0f), 1.0f); 
		}
		else if (EffectTag == FGameplayTag::RequestGameplayTag("SynergySystem.Effect.Corrosion.Erode"))
		{
			IconWidget->StatusBarIconTint = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f); 
		}
		else if (EffectTag == FGameplayTag::RequestGameplayTag("SynergySystem.Effect.Frost.Chill"))
		{
			IconWidget->StatusBarIconTint = FLinearColor(0.0f, 0.0f, 1.0f, 1.0f); 
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

		//UE_LOG(LogTemp, Warning, TEXT("Removed Effect: %s from Status Effect Bar"), *EffectTag.ToString());
	}
}
