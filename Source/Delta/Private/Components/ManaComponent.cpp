// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ManaComponent.h"

#include "Characters/DeltaBaseCharacter.h"
#include "GameStates/DeltaGameState.h"

UManaComponent::UManaComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CurrentMana = MaxMana / 2.0f;

}

void UManaComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerDeltaCharacter = OwnerDeltaCharacter ? OwnerDeltaCharacter : Cast<ADeltaBaseCharacter>(GetOwner());
	if (!OwnerDeltaCharacter) return;
	
	GetWorld()->GetTimerManager().SetTimer(ManaRecoveryTimer, this, &ThisClass::ManaRecoveryOnce, TickRate, true);
	
}

void UManaComponent::BeginDestroy()
{
	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerPending(ManaRecoveryTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(ManaRecoveryTimer);
	}
	
	Super::BeginDestroy();
}

void UManaComponent::SetCurrentMana(const float NewMana)
{
	CurrentMana = FMath::Clamp(NewMana, 0.0f, MaxMana);
	
	OnManaChanged.Broadcast(CurrentMana, MaxMana);
}

bool UManaComponent::CanUseSkill(const float Amount) const
{
	return CurrentMana > Amount || FMath::IsNearlyEqual(CurrentMana, Amount);
}

void UManaComponent::UseSkill(const float Amount)
{
	SetCurrentMana(CurrentMana - Amount);
}

void UManaComponent::ManaRecoveryOnce()
{
	SetCurrentMana(CurrentMana + ManaPerSecond * TickRate);
}
