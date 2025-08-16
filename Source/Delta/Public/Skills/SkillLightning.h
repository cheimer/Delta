// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillCollisionAttack.h"
#include "Skills/SkillBase.h"
#include "SkillLightning.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API USkillLightning : public USkillBase
{
	GENERATED_BODY()

public:
	USkillLightning();
	
	virtual void BeginSkill(UCombatComponent* InCombatComponent) override;
	virtual void EndSkill() override;

protected:
	void HitOverlapActor();
	
	UPROPERTY(EditAnywhere, Category = "Values")
	float TickRate = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Name")
	FName CollisionName = "Lightning";

	UPROPERTY(EditAnywhere, Category = "Values")
	float SkillDamage = 1.0f;
	
	TWeakObjectPtr<UBoxComponent> SkillCollisionBox;

private:
	FTimerHandle HitOverlapTimer;
	
};
