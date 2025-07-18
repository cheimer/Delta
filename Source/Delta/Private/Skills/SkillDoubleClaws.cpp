// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillDoubleClaws.h"

#include "Characters/DeltaBaseCharacter.h"

USkillDoubleClaws::USkillDoubleClaws()
{
	SkillType = EDeltaSkillType::DoubleClaws;
	
}

void USkillDoubleClaws::BeginSkill(UCombatComponent* InCombatComponent)
{
	// First Claw
	if (!bDoFirstClaw)
	{
		bDoFirstClaw = true;
	}
	// Second Claw
	else
	{
		CollisionName = SecondCollisionName;
	}
	
	Super::BeginSkill(InCombatComponent);
}
