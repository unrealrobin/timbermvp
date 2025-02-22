// Property of Paracosm Industries. Dont use my shit.


#include "Character/Enemies/TutorialDummy.h"


// Sets default values
ATutorialDummy::ATutorialDummy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATutorialDummy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATutorialDummy::HandleDummyDestroy()
{
	//Drop a specific amount of parts (Special parts with no collision at first.)

	//Destroy
}

