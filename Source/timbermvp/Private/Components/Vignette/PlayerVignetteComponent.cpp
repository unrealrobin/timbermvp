// Property of Paracosm.


#include "Components/Vignette/PlayerVignetteComponent.h"

#include "Components/PostProcessComponent.h"


// Sets default values for this component's properties
UPlayerVignetteComponent::UPlayerVignetteComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UPlayerVignetteComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (!Owner) return;

	VignettePostProcessComponent = NewObject<UPostProcessComponent>();
	if (!VignettePostProcessComponent) return;

	VignettePostProcessComponent->RegisterComponent();
	VignettePostProcessComponent->bUnbound = true;

	USceneComponent* ActorRootComponent = Owner->GetRootComponent();
	if (ActorRootComponent)
	{
		VignettePostProcessComponent->AttachToComponent(ActorRootComponent, FAttachmentTransformRules::KeepRelativeTransform);	
	}

	InitializeVignetteDynamicMaterial();
}

FDamageVignetteState UPlayerVignetteComponent::GetCurrentDamageVignetteState()
{
	FDamageVignetteState OutState = FDamageVignetteState();
	
	if (DamageVignetteMid)
	{
		FHashedMaterialParameterInfo DamageIntensityParam(TEXT("DamageIntensity"));
		FHashedMaterialParameterInfo EdgeFalloffParam(TEXT("EdgeFalloff"));
		FHashedMaterialParameterInfo DamageColorParam(TEXT("DamageColor"));
		
		DamageVignetteMid->GetScalarParameterValue(DamageIntensityParam, OutState.DamageIntensity);
		DamageVignetteMid->GetScalarParameterValue(EdgeFalloffParam, OutState.EdgeFalloff);
		DamageVignetteMid->GetVectorParameterValue(DamageColorParam, OutState.VignetteColor);
	}
	
	return OutState;
}

void UPlayerVignetteComponent::InitializeVignetteDynamicMaterial()
{
	if (DamageVignetteMaterialBase)
	{
		DamageVignetteMid = UMaterialInstanceDynamic::Create(DamageVignetteMaterialBase, this);
	}
}

void UPlayerVignetteComponent::HandleHealthChange(float HealthPercent)
{
	//TODO:: Called when player takes damage or gains health.
	
	//CHecks health percent
	CurrentState = GetCurrentDamageVignetteState();

	if (FMath::IsWithin(HealthPercent, .8, 1.0))
	{
		LerpDamageVignetteState(CurrentState, FullState, 1.0f);
	}
	else if (FMath::IsWithin(HealthPercent, .5, .8))
	{
		LerpDamageVignetteState(CurrentState, LightState, 1.0f);
	}
	else if (FMath::IsWithin(HealthPercent, .25, .5))
	{
		LerpDamageVignetteState(CurrentState, HighState, 1.0f);
	}
	else if (FMath::IsWithin(HealthPercent, 0.0, .25))
	{
		LerpDamageVignetteState(CurrentState, CriticalState, 1.0f);
	}
	else
	{
		//Edge Case. Doesnt show any Vignette, like full Health.
		LerpDamageVignetteState(CurrentState, FullState, 1.0f);
	}
	
}

void UPlayerVignetteComponent::LerpDamageVignetteState(FDamageVignetteState From, FDamageVignetteState To, float Time)
{
	FTimerHandle LerpTimerHandle;

	//Handle getting Alpha and lerping From -> To states.
}

