// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skills/SkillBase.h"
#include "SkillSpitFire.generated.h"

class AProjectileBase;
class UNiagaraSystem;
class ADeltaBaseCharacter;
/**
 * 
 */
UCLASS()
class DELTA_API USkillSpitFire : public USkillBase
{
	GENERATED_BODY()

public:
	USkillSpitFire();
	
	virtual void BeginSkill(UCombatComponent* InCombatComponent) override;

protected:
	TWeakObjectPtr<AProjectileBase> SpawnedProjectile;

private:
	UPROPERTY(EditAnywhere, Category = "Skill")
	float SkillDamage = 50;
	
	UPROPERTY(EditAnywhere, Category = "Name")
	FName SpawnSocketName = FName("SpitFireSocket");

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<AProjectileBase> FireProjectileClass;

	TWeakObjectPtr<ADeltaBaseCharacter> OwnerDeltaCharacter;
	
};
