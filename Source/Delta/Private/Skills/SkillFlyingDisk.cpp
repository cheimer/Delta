// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillFlyingDisk.h"

USkillFlyingDisk::USkillFlyingDisk()
{
	SkillType = EDeltaSkillType::FlyingDisk;
	SpawnSocketName = FName("FlyingDiskSocket");
}

void USkillFlyingDisk::BeginSkill(UCombatComponent* InCombatComponent)
{
	Super::BeginSkill(InCombatComponent);

	if (!SpawnedProjectile.IsValid()) return;

	
}
