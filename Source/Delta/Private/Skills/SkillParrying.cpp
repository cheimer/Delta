// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillParrying.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/CombatComponent.h"

void USkillParrying::BeginSkill(UCombatComponent* InCombatComponent)
{
	Super::BeginSkill(InCombatComponent);

	if (!CombatComponent.IsValid()) return;
	CombatComponent->SetDamageTakenMultiplier(0.0f);
}

void USkillParrying::ReactDamaged()
{
	Super::ReactDamaged();

	if (!CombatComponent.IsValid() || !CombatComponent->GetOwner()) return;

	FVector SpawnLocation = CombatComponent->GetOwner()->GetActorLocation();
	FRotator SpawnRotation = CombatComponent->GetOwner()->GetActorRotation();
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ParryingVFX, SpawnLocation, SpawnRotation);
}

void USkillParrying::EndSkill()
{
	Super::EndSkill();
	
	if (!CombatComponent.IsValid()) return;
	CombatComponent->SetDamageTakenMultiplier(1.0f);
}
