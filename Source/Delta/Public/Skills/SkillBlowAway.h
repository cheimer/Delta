// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skills/SkillBase.h"
#include "SkillBlowAway.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API USkillBlowAway : public USkillBase
{
	GENERATED_BODY()

public:
	USkillBlowAway();

	virtual void BeginSkill(UCombatComponent* InCombatComponent) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Values")
	float KnockbackPower = 10000.0f;
	
	UPROPERTY(EditAnywhere, Category = "Values")
	float Range = 300.0f;

private:
	TSet<TWeakObjectPtr<AActor>> OverlappedActors;
};
