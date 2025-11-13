// Fill out your copyright notice in the Description page of Project Settings.


#include "PostProcess/PostProcessHolder.h"

#include "Components/DamageVignetteComponent.h"
#include "Components/PostProcessComponent.h"

APostProcessHolder::APostProcessHolder()
{
	PrimaryActorTick.bCanEverTick = false;

	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
	SetRootComponent(PostProcessComponent);
	PostProcessComponent->bUnbound = true;
	
	DamageVignetteComponent = CreateDefaultSubobject<UDamageVignetteComponent>(TEXT("DamageVignetteComponent"));
	
}

void APostProcessHolder::RegisterMaterial(UMaterialInstanceDynamic* RegisterMI)
{
	PostProcessComponent->AddOrUpdateBlendable(RegisterMI);
}

void APostProcessHolder::UpdateDamageVignette(const float HealthRate)
{
	if (!DamageVignetteComponent) return;
	DamageVignetteComponent->UpdateDamageVignette(HealthRate);
}
