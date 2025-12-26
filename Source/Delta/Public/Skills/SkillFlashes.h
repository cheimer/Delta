// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skills/SkillCollisionAttack.h"
#include "SkillFlashes.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class DELTA_API USkillFlashes : public USkillCollisionAttack
{
	GENERATED_BODY()

public:
	USkillFlashes();
	
	virtual void BeginSkill(UCombatComponent* InCombatComponent) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Values")
	FVector MoveLocation = FVector(100.0f, 0.0f, 0.0f);

};
