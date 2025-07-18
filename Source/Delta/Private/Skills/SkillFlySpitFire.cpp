// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillFlySpitFire.h"

#include "Characters/DeltaBaseCharacter.h"
#include "Components/CombatComponent.h"
#include "Skills/Projectiles/ProjectileBase.h"

USkillFlySpitFire::USkillFlySpitFire()
{
	SkillType = EDeltaSkillType::FlySpitFire;
}

void USkillFlySpitFire::BeginSkill(UCombatComponent* InCombatComponent)
{
	Super::BeginSkill(InCombatComponent);
	
	if (!SpawnedProjectile.Get() || !InCombatComponent) return;

	FVector TargetLocation = InCombatComponent->GetSkillTargetLocation(true);
	FVector SpawnLocation = SpawnedProjectile->GetActorLocation();
	FVector Direction = (TargetLocation - SpawnLocation).GetSafeNormal();
	
	SpawnedProjectile->SetDirection(Direction);
}
