// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillLightning.h"

#include "Characters/DeltaBaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CombatComponent.h"
#include "Controllers/DeltaAIController.h"

USkillLightning::USkillLightning()
{
	SkillType = EDeltaSkillType::Lightning;
}

void USkillLightning::BeginSkill(UCombatComponent* InCombatComponent)
{
	Super::BeginSkill(InCombatComponent);
	
	if (!InCombatComponent) return;

	ADeltaBaseCharacter* OwnerDeltaCharacter = Cast<ADeltaBaseCharacter>(InCombatComponent->GetOwner());
	if (!OwnerDeltaCharacter) return;

	InCombatComponent->LookAtCameraCenter();

	SkillCollisionBox = OwnerDeltaCharacter->FindSkillCollision(CollisionName);
	if (!SkillCollisionBox.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("SkillCollisionBox is invalid"));
		return;
	}

	if (ADeltaAIController* AIController = Cast<ADeltaAIController>(OwnerDeltaCharacter->GetController()))
	{
		AIController->RotateToTarget();
	}

	GetWorld()->GetTimerManager().SetTimer(HitOverlapTimer, this, &USkillLightning::HitOverlapActor, TickRate, true);
}

void USkillLightning::EndSkill()
{
	Super::EndSkill();

	if (!CombatComponent.IsValid()) return;

	CombatComponent->LookAtForward();

	GetWorld()->GetTimerManager().ClearTimer(HitOverlapTimer);
	
}

void USkillLightning::HitOverlapActor()
{
	if (!CombatComponent.IsValid() || !CombatComponent->GetOwner()) return;

	TArray<AActor*> OverlappedActors;
	SkillCollisionBox->GetOverlappingActors(OverlappedActors);
	
	for (auto OverlapActor : OverlappedActors)
	{
		if (!OverlapActor) continue;
		if (OverlapActor == CombatComponent->GetOwner()) continue;
		
		CombatComponent->ApplySkillDamage(OverlapActor, CombatComponent->GetOwner(), SkillDamage);
	}
	
}
