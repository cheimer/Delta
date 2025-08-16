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

protected:
	AProjectileFlyingDisk();
	
	virtual void BeginPlay() override;
	
};
