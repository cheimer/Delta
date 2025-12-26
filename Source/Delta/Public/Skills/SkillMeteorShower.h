// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skills/SkillBase.h"
#include "SkillMeteorShower.generated.h"

class ADeltaBaseCharacter;
class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class DELTA_API USkillMeteorShower : public USkillBase
{
	GENERATED_BODY()

public:
	USkillMeteorShower();
	
	virtual void BeginSkill(UCombatComponent* InCombatComponent) override;

	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* MeteorShowerVFX;

	UPROPERTY(EditAnywhere, Category = "Values")
	float SpawnHeight = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Values")
	float SpawnDuration = 5.0f;

private:
	UPROPERTY(EditAnywhere, Category = "Skill")
	float SkillDamage = 5.0f;
	
	UFUNCTION()
	void AttackUnderEnemy();

	FVector SpawnLocation;
	float SpawnTime = 0.0f;
	
	FTimerHandle AttackTimerHandle;

	//(SpawnHeight / FallingSpeed) ~ (VFX Duration + SpawnHeight / FallingSpeed) = Hit Time
	static constexpr float FallingSpeed = 1000.0f;
	static constexpr float TickRate = 0.5f;
	
};
