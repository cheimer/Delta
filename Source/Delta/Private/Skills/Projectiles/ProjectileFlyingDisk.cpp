// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/Projectiles/ProjectileFlyingDisk.h"

#include "Characters/DeltaBaseCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectileFlyingDisk::AProjectileFlyingDisk()
{
	ProjectileDuration = 10.0f;
	
}

void AProjectileFlyingDisk::BeginPlay()
{
	Super::BeginPlay();

	if (ProjectileMovement)
	{
		ProjectileMovement->ProjectileGravityScale = 0.0f;
		ProjectileMovement->bIsHomingProjectile = true;
		ProjectileMovement->HomingAccelerationMagnitude = 1500.0f;
	}
	
}

void AProjectileFlyingDisk::SetBeginSkill(AActor* SpawnerActor, UCombatComponent* InCombatComponent, const FTransform& SpawnTransform, const float SkillDamage)
{
	Super::SetBeginSkill(SpawnerActor, InCombatComponent, SpawnTransform, SkillDamage);
	
	if (!InCombatComponent || !ProjectileMovement) return;

	ADeltaBaseCharacter* TargetActor = InCombatComponent->GetSkillTargetActor();
	if (!TargetActor) return;

	ProjectileMovement->HomingTargetComponent = TargetActor->GetRootComponent();
	
}

void AProjectileFlyingDisk::ReturnPool()
{
	if (ProjectileMovement)
	{
		ProjectileMovement->HomingTargetComponent.Reset();
	}
	
	Super::ReturnPool();
}
