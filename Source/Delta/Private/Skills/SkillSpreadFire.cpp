// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillSpreadFire.h"

#include "Characters/DeltaBaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CombatComponent.h"
#include "Skills/Projectiles/ProjectileBase.h"

USkillSpreadFire::USkillSpreadFire()
{
	SkillType = EDeltaSkillType::SpreadFire;
}

void USkillSpreadFire::BeginSkill(UCombatComponent* InCombatComponent)
{
	Super::BeginSkill(InCombatComponent);
	
	SkillCollisionBox->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnTargetEscape);

	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ThisClass::OnAttackOverlapped, TickRate, true, 0.0f);

}

void USkillSpreadFire::EndSkill()
{
	Super::EndSkill();

	SkillCollisionBox->OnComponentEndOverlap.RemoveDynamic(this, &ThisClass::OnTargetEscape);
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
}

void USkillSpreadFire::OnTargetOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || !CombatComponent.IsValid() || !CombatComponent->GetIsOpponent(OtherActor)) return;

	if (OverlappedActors.Contains(OtherActor)) return;

	OverlappedActors.Add(OtherActor);
}

void USkillSpreadFire::OnTargetEscape(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor || !CombatComponent.IsValid()) return;

	if (!OverlappedActors.Contains(OtherActor)) return;

	OverlappedActors.Remove(OtherActor);
}

void USkillSpreadFire::OnAttackOverlapped()
{
	if (!CombatComponent.IsValid()) return;

	TArray<AActor*> CopyOverlappedActors;
	for (TWeakObjectPtr<AActor>& OverlappedActor: OverlappedActors)
	{
		if (OverlappedActor.IsValid())
		{
			CopyOverlappedActors.Add(OverlappedActor.Get());
		}
	}
	
	for (AActor* OverlappedActor : CopyOverlappedActors)
	{
		if (IsValid(OverlappedActor) && OverlappedActor != OwnerDeltaCharacter)
		{
			CombatComponent->ApplySkillDamage(OverlappedActor, CombatComponent->GetOwner(), SkillDamage);
		}
	}
}
