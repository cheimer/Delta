// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skills/SkillBase.h"
#include "SkillProjectile.generated.h"

class AProjectileBase;
/**
 * 
 */
UCLASS()
class DELTA_API USkillProjectile : public USkillBase
{
	GENERATED_BODY()

public:
	virtual void BeginSkill(UCombatComponent* InCombatComponent) override;

protected:
	TWeakObjectPtr<AProjectileBase> SpawnedProjectile;

	UPROPERTY(EditAnywhere, Category = "Values")
	float SkillDamage = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Name")
	FName SpawnSocketName = FName();

private:
	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<AProjectileBase> ProjectileClass;

};
