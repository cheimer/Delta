// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/Projectiles/ProjectileFlyingDisk.h"

#include "Characters/DeltaBaseCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectileFlyingDisk::AProjectileFlyingDisk()
{
	ProjectileDuration = 10.0f;
	
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	
}

void AProjectileFlyingDisk::BeginPlay()
{
	Super::BeginPlay();

	if (!CombatComponent.IsValid()) return;

	ADeltaBaseCharacter* TargetActor = CombatComponent->GetSkillTargetActor();
	if (!TargetActor) return;

	ProjectileMovement->HomingTargetComponent = TargetActor->GetRootComponent();
	
	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->HomingAccelerationMagnitude = 1500.0f;
	
}
