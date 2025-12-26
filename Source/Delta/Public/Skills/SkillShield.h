// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skills/SkillBase.h"
#include "SkillShield.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class DELTA_API USkillShield : public USkillBase
{
	GENERATED_BODY()

public:
	USkillShield();
	
	virtual void BeginSkill(UCombatComponent* InCombatComponent) override;
	virtual void ReactDamaged() override;
	virtual void EndSkill() override;

protected:
	UFUNCTION()
	void ShieldActivating();
	
	UPROPERTY(EditAnywhere, Category = "Values")
	float DurationTime = 0.5f;

private:
	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* ShieldReactVFX;

	FTimerHandle EndTimerHandle;
	bool bExitLoop = true;

	TWeakObjectPtr<UAnimInstance> AnimInstance;
	
};
