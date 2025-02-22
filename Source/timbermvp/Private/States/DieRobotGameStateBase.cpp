// Property of Paracosm Industries. Dont use my shit.


#include "States/DieRobotGameStateBase.h"

void ADieRobotGameStateBase::ChangeTutorialGameState(ETutorialState NewState)
{
	TutorialState = NewState;
	
	OnTutorialStateChange.Broadcast(NewState);

	UE_LOG(LogTemp, Warning, TEXT("Tutorial State Changed - New State: %hhd"), NewState);
}
