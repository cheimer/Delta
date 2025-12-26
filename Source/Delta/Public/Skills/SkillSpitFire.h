// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillProjectile.h"
#include "Skills/SkillBase.h"
#include "SkillSpitFire.generated.h"

class AProjectileBase;
class UNiagaraSystem;
class ADeltaBaseCharacter;
/**
 * 
 */
UCLASS()
class DELTA_API USkillSpitFire : public USkillProjectile
{
	GENERATED_BODY()

public:
	USkillSpitFire();
	
};
