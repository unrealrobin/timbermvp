// Property of Paracosm.


#include "Components/Vignette/PlayerVignetteComponent.h"

#include "Components/PostProcessComponent.h"


// Sets default values for this component's properties
UPlayerVignetteComponent::UPlayerVignetteComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
}


void UPlayerVignetteComponent::BeginDestroy()
{
	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(DamageVignetteTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(DamageVignetteTimerHandle);
	}
	
	Super::BeginDestroy();
}

// Called when the game starts
void UPlayerVignetteComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (!Owner) return;

	/*
	* - `NewObject()` without an outer parameter creates an object without proper world context
	* - When the component tries to access world-related functionality internally, it fails the `MyOwnerWorld` assertion
	 */
	VignettePostProcessComponent = NewObject<UPostProcessComponent>(Owner);
	if (!VignettePostProcessComponent) return;

	VignettePostProcessComponent->RegisterComponent();
	VignettePostProcessComponent->bUnbound = true;
	//VignettePostProcessComponent->bEnabled = true;

	USceneComponent* ActorRootComponent = Owner->GetRootComponent();
	if (ActorRootComponent)
	{
		VignettePostProcessComponent->AttachToComponent(ActorRootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		UE_LOG(LogTemp, Warning, TEXT("PlayerVignetteComponent - Attached PostProcessComponent to Actor Root Component"));
	}

	InitializeVignetteDynamicMaterial();
	
}

FDamageVignetteState UPlayerVignetteComponent::GetCurrentDamageVignetteState()
{
	FDamageVignetteState OutState;
	
	OutState.State = CurrentVignetteState;
	
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

		if (!DamageVignetteMid)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerVignetteComponent - Failed to create DamageVignetteMid Material Instance Dynamic"));
		}
		else
		{
			FWeightedBlendable DamageVignetteMidBlendable;
			DamageVignetteMidBlendable.Object = DamageVignetteMid;
			DamageVignetteMidBlendable.Weight = 1.0f;
			VignettePostProcessComponent->Settings.WeightedBlendables.Array.Add(DamageVignetteMidBlendable);

			//At this point the Material should have been added so we confirm with this check.
			if (VignettePostProcessComponent->Settings.WeightedBlendables.Array.Num() < 1)
			{
				UE_LOG(LogTemp, Warning, TEXT("PlayerVignetteComponent - Failed to add DamageVignetteMid Material Instance Dynamic to PostProcessComponent Settings"))
			}
		}
	}
}

void UPlayerVignetteComponent::HandleHealthChange(float HealthPercent)
{

	//Ensures percent values are clamped.
	HealthPercent = FMath::Clamp(HealthPercent, 0.0f, 1.0f);
	
	//CHecks health percent
	CurrentState = GetCurrentDamageVignetteState();
	FDamageVignetteState TargetState;
	
	if (HealthPercent > .8f)
	{
		TargetState = FullState;
		CurrentVignetteState = EDamageVignetteState::FullHealth;
	}
	else if (HealthPercent > .5f)
	{
		TargetState = LightState;
		CurrentVignetteState = EDamageVignetteState::LightDamage;
	}
	else if (HealthPercent > .25f)
	{
		TargetState = HighState;
		CurrentVignetteState = EDamageVignetteState::HighDamage;
	}
	else if (HealthPercent <= 0.25f)
	{
		TargetState = CriticalState;
		CurrentVignetteState = EDamageVignetteState::Critical;
	}
	else
	{
		TargetState = FullState;
		CurrentVignetteState = EDamageVignetteState::FullHealth;
	}
    
	//Only lerp if target state type is different
	if (CurrentState.State != TargetState.State)
	{
		LerpDamageVignetteState(CurrentState, TargetState, 1.0f);
	}

	
}

void UPlayerVignetteComponent::LerpDamageVignetteState(FDamageVignetteState From, FDamageVignetteState To, float Time)
{
	   // Create a shared pointer to track lerp state

	if (GetWorld()->GetTimerManager().IsTimerActive(DamageVignetteTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(DamageVignetteTimerHandle);
	}

    struct FLerpState
    {
        FDamageVignetteState FromState;
        FDamageVignetteState ToState;
        float ElapsedTime = 0.0f;
        float Duration;
    };
    
    TSharedPtr<FLerpState> LerpState = MakeShareable(new FLerpState());
    LerpState->FromState = From;
    LerpState->ToState = To;
    LerpState->Duration = Time;
	
    auto UpdateLerpLambda = [this, LerpState]()
    {
        if (!LerpState.IsValid() || !IsValid(this) || !DamageVignetteMid)
        {
	        return;
        }
    	
        // Update elapsed time
        LerpState->ElapsedTime += GetWorld()->GetDeltaSeconds();
        
        // Calculate alpha (0 to 1)
        float Alpha = FMath::Clamp(LerpState->ElapsedTime / LerpState->Duration, 0.0f, 1.0f);
        
        // Lerp the values
        float DamageIntensityValue = FMath::Lerp(LerpState->FromState.DamageIntensity, LerpState->ToState.DamageIntensity, Alpha);
        float EdgeFalloffValue = FMath::Lerp(LerpState->FromState.EdgeFalloff, LerpState->ToState.EdgeFalloff, Alpha);
        FLinearColor DamageColorValue = FMath::Lerp(LerpState->FromState.VignetteColor, LerpState->ToState.VignetteColor, Alpha);
        
        // Apply the lerped values to the material
        if (IsValid(DamageVignetteMid))
        {
            DamageVignetteMid->SetScalarParameterValue("DamageIntensity", DamageIntensityValue);
            DamageVignetteMid->SetScalarParameterValue("EdgeFalloff", EdgeFalloffValue);
            DamageVignetteMid->SetVectorParameterValue("DamageColor", DamageColorValue);
        }
        
        // Check if lerp is complete
        if (Alpha >= 1.0f)
        {
            GetWorld()->GetTimerManager().ClearTimer(DamageVignetteTimerHandle);
        }

    	//UE_LOG(LogTemp, Warning, TEXT("Lerp Damage Vignette State: %f"), Alpha);
    };
    
    // Start the timer with the lambda
    GetWorld()->GetTimerManager().SetTimer(
        DamageVignetteTimerHandle,
        FTimerDelegate::CreateLambda(UpdateLerpLambda),
        GetWorld()->GetDeltaSeconds(),
        true
    );

}

