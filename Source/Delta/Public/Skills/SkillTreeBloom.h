// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skills/SkillBase.h"
#include "SkillTreeBloom.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class DELTA_API USkillTreeBloom : public USkillBase
{
	GENERATED_BODY()

public:
	USkillTreeBloom();

	virtual void BeginSkill(UCombatComponent* InCombatComponent) override;
	virtual void EndSkill() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Values")
	float Range = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Values")
	float BlendOutTime = 0.25;
	
	UPROPERTY(EditAnywhere, Category = "Values")
	float SkillDamage = 50;
	
	UPROPERTY(EditAnywhere, Category = "Values")
	FName AttachSocketName = NAME_None;

	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* AttachVFX;
	
	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* BloomVFX;

private:
	bool bIsInterrupted = false;

	UPROPERTY()
	UNiagaraComponent* SpawnedAttachNiagara;
	
};
