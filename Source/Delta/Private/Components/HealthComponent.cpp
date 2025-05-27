// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

#include "Characters/DeltaBaseCharacter.h"
#include "Components/CombatComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CurrentHealth = MaxHealth;

	OwnerDeltaCharacter = nullptr;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerDeltaCharacter = OwnerDeltaCharacter ? OwnerDeltaCharacter : Cast<ADeltaBaseCharacter>(GetOwner());
	
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

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);
	
	if (FMath::IsNearlyZero(CurrentHealth))
	{
		bIsDead = true;
		OwnerDeltaCharacter->OnCharacterDeath.Broadcast(OwnerDeltaCharacter);
	}
}
