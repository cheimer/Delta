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
	if (!OwnerDeltaCharacter) return;
	
	CurrentHealth = MaxHealth;
	OnHealthChanged.Broadcast(OwnerDeltaCharacter, CurrentHealth, MaxHealth, false);
}

void UHealthComponent::TakeDamage(float Damage, AActor* DamageCauser)
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
	
	if (ADeltaBaseCharacter* DamageCauserCharacter = Cast<ADeltaBaseCharacter>(DamageCauser))
	{
		DamageCauserCharacter->AddTotalDealing(BeforeHealth - CurrentHealth);
	}
	
	if (!FMath::IsNearlyEqual(BeforeHealth, CurrentHealth))
	{
		OnHealthChanged.Broadcast(OwnerDeltaCharacter, CurrentHealth, MaxHealth, BeforeHealth > CurrentHealth);
	}
	
	if (FMath::IsNearlyZero(CurrentHealth))
	{
		bIsDead = true;
		OwnerDeltaCharacter->OnCharacterDeath.Broadcast(OwnerDeltaCharacter);
	}

}

bool UHealthComponent::CauseDamage(float Damage)
{
	OwnerDeltaCharacter = OwnerDeltaCharacter ? OwnerDeltaCharacter : Cast<ADeltaBaseCharacter>(GetOwner());
	
	if (!OwnerDeltaCharacter || bIsDead) return false;

	if (UCombatComponent* CombatComponent = OwnerDeltaCharacter->GetComponentByClass<UCombatComponent>())
	{
		Damage *= CombatComponent->GetDamageTakenMultiplier();
	}

	return Damage > 0;
}

float UHealthComponent::GetHealthPercentage() const
{
	return CurrentHealth / MaxHealth;
}

void UHealthComponent::SetHealthPercentage(const float HealthPercentage)
{
	OwnerDeltaCharacter = OwnerDeltaCharacter ? OwnerDeltaCharacter : Cast<ADeltaBaseCharacter>(GetOwner());
	if (!OwnerDeltaCharacter) return;
	
	float BeforeHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(MaxHealth * HealthPercentage, 0.0f, MaxHealth);

	if (!FMath::IsNearlyEqual(BeforeHealth, CurrentHealth))
	{
		OnHealthChanged.Broadcast(OwnerDeltaCharacter, CurrentHealth, MaxHealth, BeforeHealth > CurrentHealth);
	}
}

void UHealthComponent::SetMaxHealth(const float InMaxHealth)
{
	if (InMaxHealth <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetMaxHealth: Invalid value %f"), InMaxHealth);
		return;
	}
	OwnerDeltaCharacter = OwnerDeltaCharacter ? OwnerDeltaCharacter : Cast<ADeltaBaseCharacter>(GetOwner());
	if (!OwnerDeltaCharacter) return;

	const float HealthPercentage = GetHealthPercentage();
	MaxHealth = InMaxHealth;

	// If health was already initialized, maintain the same percentage
	if (CurrentHealth > 0.0f)
	{
		CurrentHealth = FMath::Clamp(MaxHealth * HealthPercentage, 0.0f, MaxHealth);
		OnHealthChanged.Broadcast(OwnerDeltaCharacter, CurrentHealth, MaxHealth, false);
	}
}
