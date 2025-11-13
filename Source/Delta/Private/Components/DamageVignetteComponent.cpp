// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DamageVignetteComponent.h"

#include "PostProcess/PostProcessHolder.h"

UDamageVignetteComponent::UDamageVignetteComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UDamageVignetteComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!GetWorld() || !DamageVignetteMI || !GetOwner()) return;

	DamageVignetteMaterialDynamic = UMaterialInstanceDynamic::Create(DamageVignetteMI, nullptr);
	if (!DamageVignetteMaterialDynamic)
	{
		UE_LOG(LogTemp, Warning, TEXT("Damage Vignette MI isn't created. Check %s"), *GetOwner()->GetName());
		return;
	}

	DamageVignetteMaterialDynamic->SetScalarParameterValue(InnerVignette, 1.0f);

	if (APostProcessHolder* PostProcessHolder = Cast<APostProcessHolder>(GetOwner()))
	{
		PostProcessHolder->RegisterMaterial(DamageVignetteMaterialDynamic);
	}
}

void UDamageVignetteComponent::UpdateDamageVignette(const float HealthRate)
{
	if (!DamageVignetteMaterialDynamic) return;

	DamageVignetteMaterialDynamic->SetScalarParameterValue(InnerVignette, HealthRate);
}
