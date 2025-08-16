// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillBase.h"

#include "Components/CombatComponent.h"

void USkillBase::BeginSkill(UCombatComponent* InCombatComponent)
{
	if (!InCombatComponent) return;
	
	CombatComponent = InCombatComponent;
}

void USkillBase::ReactDamaged()
{
}

void USkillBase::ActInput()
{
}

void USkillBase::EndSkill()
{
	
}
