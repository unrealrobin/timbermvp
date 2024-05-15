// Property of Paracosm Industries. Dont use my shit.


#include "UI/TimberHUDBase.h"
#include "Blueprint/UserWidget.h"

void ATimberHUDBase::BeginPlay()
{
	Super::BeginPlay();

	if(RootWidgetClass)
	{
		RootWidget = CreateWidget<UUserWidget>(GetWorld(), RootWidgetClass);
		if(RootWidget)
		{
			RootWidget->AddToViewport(1);
		}
	}
	
	
}
