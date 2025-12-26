// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skills/Projectiles/ProjectileBase.h"
#include "ProjectileFlyingDisk.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API AProjectileFlyingDisk : public AProjectileBase
{
	GENERATED_BODY()

public:
	virtual void SetBeginSkill(AActor* SpawnerActor, UCombatComponent* InCombatComponent, const FTransform& SpawnTransform, const float SkillDamage) override;

protected:
	AProjectileFlyingDisk();
	
	virtual void BeginPlay() override;

	virtual void ReturnPool() override;
	
};
