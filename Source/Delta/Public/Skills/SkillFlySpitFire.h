// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillSpitFire.h"
#include "Skills/SkillBase.h"
#include "SkillFlySpitFire.generated.h"

class AProjectileBase;
/**
 * 
 */
UCLASS()
class DELTA_API USkillFlySpitFire : public USkillSpitFire
{
	GENERATED_BODY()
	
public:
	USkillFlySpitFire();
	
	virtual void BeginSkill(UCombatComponent* InCombatComponent) override;

};
