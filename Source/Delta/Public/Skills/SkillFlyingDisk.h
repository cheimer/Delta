// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skills/SkillProjectile.h"
#include "SkillFlyingDisk.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API USkillFlyingDisk : public USkillProjectile
{
	GENERATED_BODY()

public:
	USkillFlyingDisk();
	
	virtual void BeginSkill(UCombatComponent* InCombatComponent) override;

};
