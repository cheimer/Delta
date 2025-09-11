// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillTreeBloom.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/DeltaBaseCharacter.h"
#include "Components/CombatComponent.h"

USkillTreeBloom::USkillTreeBloom()
{
	SkillType = EDeltaSkillType::TreeBloom;
}

void USkillTreeBloom::BeginSkill(UCombatComponent* InCombatComponent)
{
	Super::BeginSkill(InCombatComponent);

	if (!CombatComponent.IsValid() && !CombatComponent->GetOwner()) return;
	AActor* OwnerActor = CombatComponent->GetOwner();
	
	const float Distance = FVector::Distance(OwnerActor->GetActorLocation(), InCombatComponent->GetSkillTargetLocation(true));
	
	if (Distance > Range)
	{
		bIsInterrupted = true;
		CombatComponent->CancelSkill(BlendOutTime);
	}
	else
	{
		bIsInterrupted = false;

		if (AttachVFX && CombatComponent->GetSkillTargetActor() && CombatComponent->GetSkillTargetActor()->GetMesh())
		{
			SpawnedAttachNiagara = UNiagaraFunctionLibrary::SpawnSystemAttached(AttachVFX, CombatComponent->GetSkillTargetActor()->GetMesh(),
				AttachSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
			if (SpawnedAttachNiagara)
			{
				SpawnedAttachNiagara->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
			}
		}
	}
}

void USkillTreeBloom::EndSkill()
{
	Super::EndSkill();

	if (bIsInterrupted) return;

	if (!CombatComponent.IsValid() || !CombatComponent->GetSkillTargetActor()) return;
	FVector SpawnLocation = CombatComponent->GetSkillTargetActor()->GetActorLocation();

	if (BloomVFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloomVFX, SpawnLocation, FRotator::ZeroRotator, FVector(0.4f, 0.4f, 0.4f));
	}

	if (!SpawnedAttachNiagara) return;
	SpawnedAttachNiagara->DestroyInstance();
}
