// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

#include "Characters/DeltaBaseCharacter.h"
#include "Components/CombatComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	OwnerDeltaCharacter = nullptr;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerDeltaCharacter = OwnerDeltaCharacter ? OwnerDeltaCharacter : Cast<ADeltaBaseCharacter>(GetOwner());
	
	CurrentHealth = MaxHealth;
}

void UHealthComponent::TakeDamage(float Damage)
{
	OwnerDeltaCharacter = OwnerDeltaCharacter ? OwnerDeltaCharacter : Cast<ADeltaBaseCharacter>(GetOwner());
	
	if (!OwnerDeltaCharacter || bIsDead) return;

	if (UCombatComponent* CombatComponent = OwnerDeltaCharacter->GetComponentByClass<UCombatComponent>())
	{
		Damage *= CombatComponent->GetDamageTakenMultiplier();
		CombatComponent->TakeDamage();
	}

	float BeforeHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);
	
	if (!FMath::IsNearlyEqual(BeforeHealth, CurrentHealth))
	{
		OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, BeforeHealth > CurrentHealth);
	}
	
	if (FMath::IsNearlyZero(CurrentHealth))
	{
		bIsDead = true;
		OwnerDeltaCharacter->OnCharacterDeath.Broadcast(OwnerDeltaCharacter);
	}
}

float UHealthComponent::GetHealthPercentage() const
{
	return CurrentHealth / MaxHealth;
}

void UHealthComponent::SetHealthPercentage(const float HealthPercentage)
{
	float BeforeHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(MaxHealth * HealthPercentage, 0.0f, MaxHealth);
	
	if (!FMath::IsNearlyEqual(BeforeHealth, CurrentHealth))
	{
		OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, BeforeHealth > CurrentHealth);
	}
	
	
}
