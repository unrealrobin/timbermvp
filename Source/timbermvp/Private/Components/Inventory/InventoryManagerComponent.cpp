// Property of Paracosm Industries. Dont use my shit.


#include "Components/Inventory/InventoryManagerComponent.h"

#include "SWarningOrErrorBox.h"
#include "BuildSystem/BuildableBase.h"
#include "Character/TimberPlayableCharacter.h"
#include "Controller/TimberPlayerController.h"


UInventoryManagerComponent::UInventoryManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	LoadOwningPlayerState();
}

void UInventoryManagerComponent::LoadOwningPlayerState()
{

	/*
	 * PCH - Player Character
	 * PC - Player Controller
	 * PS - Player State
	 */
	ATimberPlayableCharacter* PCh = Cast<ATimberPlayableCharacter>(GetOwner());
	if(PCh)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory Manager Component - Player Character Loaded from GetOwner()"));
		ATimberPlayerController* PC = Cast<ATimberPlayerController>(PCh->GetController());
		if(PC)
		{
			UE_LOG(LogTemp, Warning, TEXT("Inventory Manager Component - Player Controller Loaded."));
			PS = PC->GetPlayerState<APlayerStateBase>();
			if(PS == nullptr )
			{
				UE_LOG(LogTemp, Warning, TEXT("Inventory Manager Component - Player State Cast Failed."));
			}
			if(PS)
			{
				UE_LOG(LogTemp, Warning, TEXT("Player State Loaded for the Inventory Management Component."))
			}
		}
	}
}

void UInventoryManagerComponent::AddPartsToInventory(int PartsToAdd)
{
	if(PS)
	{
		
		PS->MainInventory->NumberOfParts += PartsToAdd;
		UE_LOG(LogTemp, Warning, TEXT("Part Added to Inventory. Parts In Inventory = %d"), PS->MainInventory->NumberOfParts);

		UpdateInventoryHandle.Broadcast();
	}
}

void UInventoryManagerComponent::RemovePartsFromInventory(int PartsToRemove)
{
	if(PartsToRemove)
	{
		PS->MainInventory->NumberOfParts -= PartsToRemove;
		UpdateInventoryHandle.Broadcast();
	}
} 

bool UInventoryManagerComponent::bCanAffordCost(FBuildableCost CostOfBuildable)
{
	

	return false;
}

int UInventoryManagerComponent::GetPartsInInventory()
{
	if(PS)
	{
		return PS->MainInventory->NumberOfParts;
	}
	else
	{
		return 9999999;
	}
}

