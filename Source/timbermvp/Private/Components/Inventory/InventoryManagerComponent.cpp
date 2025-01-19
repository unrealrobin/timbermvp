// Property of Paracosm Industries. Dont use my shit.


#include "Components/Inventory/InventoryManagerComponent.h"

#include "Character/TimberPlayableCharacter.h"


UInventoryManagerComponent::UInventoryManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	/*
	 * PCH - Player Character
	 * PC - Player Controller
	 * PS - Player State
	 */
	ATimberPlayableCharacter* PCh = Cast<ATimberPlayableCharacter>(GetOwner());
	if(PCh)
	{
		AController* PC = PCh->GetController();
		if(PC)
		{
			PS = PC->GetPlayerState<APlayerStateBase>();
			if(PS)
			{
				UE_LOG(LogTemp, Warning, TEXT("Player State Loaded for the Inventory Management Component."))
			}
		}
	}
	

}

void UInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryManagerComponent::AddPartsToInventory(int PartsToAdd)
{
	if(PS)
	{
		PS->MainInventory.NumberOfParts += PartsToAdd;
	}
}

void UInventoryManagerComponent::RemovePartsFromInventory(int PartsToRemove)
{
	if(bCanAffordPartsCost(PartsToRemove))
	{
		PS->MainInventory.NumberOfParts -= PartsToRemove;
	}
} 

bool UInventoryManagerComponent::bCanAffordPartsCost(int CostOfParts)
{
	if(GetPartsInInventory() > CostOfParts) return true;

	return false;
}

void UInventoryManagerComponent::TickComponent(
	float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

int UInventoryManagerComponent::GetPartsInInventory()
{
	if(PS)
	{
		return PS->MainInventory.NumberOfParts;
	}
	else
	{
		return 9999999;
	}
}

