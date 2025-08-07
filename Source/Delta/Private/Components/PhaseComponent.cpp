// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PhaseComponent.h"

#include "Components/HealthComponent.h"

UPhaseComponent::UPhaseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UPhaseComponent::BeginPlay()
{
	Super::BeginPlay();

	PhaseTransRates.Sort([](int A, int B)
	{
		return A > B;
	});

	if (GetOwner())
	{
		if (UHealthComponent* HealthComp = GetOwner()->FindComponentByClass<UHealthComponent>())
		{
			HealthComp->OnHealthChanged.AddDynamic(this, &ThisClass::HandleHealthChanged);
		}
	}

	
}

void UPhaseComponent::HandleHealthChanged(float CurrentHealth, float MaxHealth, bool bIsDamaged)
{
	bool bDoNextPhase =
		bIsDamaged &&
		PhaseTransRates.Num() > CurrentPhase &&
		CurrentHealth / MaxHealth < PhaseTransRates[CurrentPhase];

	if (bDoNextPhase)
	{
		CurrentPhase++;
		OnPhaseChanged.Broadcast(CurrentPhase);
	}

}
