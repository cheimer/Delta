// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skills/SkillBase.h"
#include "SkillForestBoom.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class DELTA_API USkillForestBoom : public USkillBase
{
	GENERATED_BODY()

public:
	USkillForestBoom();

	virtual void BeginSkill(UCombatComponent* InCombatComponent) override;
	virtual void EndSkill() override;

protected:
	void SpawnGroundVFX();
	void SpawnBoomVFX();
	
	UPROPERTY(EditAnywhere, Category = "Values")
	float SkillDamage = 30.0f;
	
	UPROPERTY(EditAnywhere, Category = "Values")
	float AOERadius = 200.0f;
	
	UPROPERTY(EditAnywhere, Category = "Values")
	float SpawnDuration = 1.0f;
	
	UPROPERTY(EditAnywhere, Category = "Values")
	int32 SpawnNum = 5;

	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* GroundVFX;
	
	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* BoomVFX;

private:
	static constexpr float DefaultRadius = 80.0f;
	
	UPROPERTY()
	TArray<UNiagaraComponent*> SpawnedGroundNiagaras;

	TArray<FVector> TargetLocations;

	FTimerHandle SpawnGroundTimer;
	FTimerHandle SpawnBoomTimer;

	int SpawnBoomIndex = 0;

};
