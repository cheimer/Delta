// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillFlashes.h"

#include "Characters/DeltaBaseCharacter.h"
#include "Components/CombatComponent.h"

USkillFlashes::USkillFlashes()
{
	SkillType = EDeltaSkillType::Flashes;
	CollisionName = FName("Flashes");
}

void USkillFlashes::BeginSkill(UCombatComponent* InCombatComponent)
{
	Super::BeginSkill(InCombatComponent);
	
	if (!InCombatComponent) return;

	if (InCombatComponent->IsFirstSection().Get(false))
	{
		InCombatComponent->MoveCharacterMesh(MoveLocation);
	}

}
