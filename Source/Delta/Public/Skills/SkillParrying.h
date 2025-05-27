// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skills/SkillBase.h"
#include "SkillParrying.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class DELTA_API USkillParrying : public USkillBase
{
	GENERATED_BODY()

public:
	virtual void BeginSkill(UCombatComponent* InCombatComponent) override;
	virtual void ReactDamaged() override;
	virtual void EndSkill() override;
	
private:
	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* ParryingVFX;
	
};
