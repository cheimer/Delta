// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillArrowShot.h"

#include "Components/CombatComponent.h"
#include "Skills/Projectiles/ProjectileBase.h"

USkillArrowShot::USkillArrowShot()
{
	SkillType = EDeltaSkillType::ArrowShot;
	SpawnSocketName = FName("ArrowShotSocket");
}

void USkillArrowShot::BeginSkill(UCombatComponent* InCombatComponent)
{
	Super::BeginSkill(InCombatComponent);

	if (!InCombatComponent) return;
	if (!SpawnedProjectile.IsValid()) return;
	
	if (InCombatComponent->GetCurrentSkillCount() + 1 == MaxSkillCount)
	{
		SpawnedProjectile->SetActorScale3D(FVector::One() * 3.0f);
		SpawnedProjectile->SetGravityScale(0.0f);
		SpawnedProjectile->SetProjectileDamage(SkillDamage * LastSkillDamageMul);
	}
}
