// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skills/SkillBase.h"
#include "SkillRailGun.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;

/**
 * 
 */
UCLASS()
class DELTA_API USkillRailGun : public USkillBase
{
	GENERATED_BODY()

public:
	USkillRailGun();

	virtual void BeginSkill(UCombatComponent* InCombatComponent) override;

protected:
	void MoveVFX();

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UNiagaraSystem* RailGunVFX;

	UPROPERTY(EditDefaultsOnly, Category = "Values")
	float VFXSpeed = 2500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Values")
	float SpawnDuration = 1.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Values")
	float SkillDamage = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	FName SpawnSocketName = FName("RailGunSocket");

private:
	UPROPERTY()
	UNiagaraComponent* SpawnVFX;
	
	FTimerHandle VFXTimer;

	float CurrentSpawnedTime = 0.0f;

};
