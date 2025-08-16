// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillProjectile.h"
#include "Skills/SkillBase.h"
#include "SkillArrowShot.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API USkillArrowShot : public USkillProjectile
{
	GENERATED_BODY()

public:
	USkillArrowShot();
	
	virtual void BeginSkill(UCombatComponent* InCombatComponent) override;

private:
	UPROPERTY(EditAnywhere, Category = "Values")
	int32 MaxSkillCount = 4;

};
