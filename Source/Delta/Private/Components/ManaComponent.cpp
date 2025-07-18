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
	StartManaRecoveryTimer();
	
}

void UManaComponent::UseSkill(const float Amount)
{
	CurrentMana = FMath::Clamp(CurrentMana - Amount, 0.0f, MaxMana);
}

void UManaComponent::StartManaRecoveryTimer()
{
	if (!GetWorld()) return;
	
	GetWorld()->GetTimerManager().SetTimer(ManaRecoveryTimer, this, &ThisClass::ManaRecoveryOnce, ManaRecoveryTime, true);
}

void UManaComponent::StopManaRecoveryTimer()
{
	if (!GetWorld()) return;
	
	GetWorld()->GetTimerManager().ClearTimer(ManaRecoveryTimer);
}


void UManaComponent::ManaRecoveryOnce()
{
	ADeltaGameState* GameState = GetWorld()->GetGameState<ADeltaGameState>();
	
	if (!GameState || GameState->GetIsGamePaused()) return;
	
	CurrentMana = FMath::Clamp(CurrentMana + 1.0f, 0.0f, MaxMana);
}
