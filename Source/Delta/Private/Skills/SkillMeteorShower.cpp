// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillMeteorShower.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Characters/DeltaBaseCharacter.h"
#include "Components/CombatComponent.h"
#include "Kismet/GameplayStatics.h"

void USkillMeteorShower::BeginSkill(UCombatComponent* InCombatComponent)
{
	Super::BeginSkill(InCombatComponent);
	
	if (!InCombatComponent || !InCombatComponent->GetOwner()) return;
	if (!MeteorShowerVFX)
	{
		UE_LOG(LogTemp, Warning, TEXT("MeteorShow VFX is not settings"));
		return;
	}

	TargetCharacter = InCombatComponent->GetSkillTargetActor();
	
	SpawnLocation = InCombatComponent->GetSkillTargetLocation(false) + FVector::UpVector * SpawnHeight;
	SpawnLocation.Z = InCombatComponent->GetOwner()->GetActorLocation().Z + SpawnHeight;
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, MeteorShowerVFX, SpawnLocation);
	
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ThisClass::AttackUnderEnemy, TickRate, true, SpawnHeight / FallingSpeed);
	SpawnTime += SpawnHeight / FallingSpeed;
}

/*
 *	It is work on 1 target enemy.
 *	if wanna hit multi, Distance TargetCharacter to SpawnLocation -> LineTrace or Add HitBox
 */
void USkillMeteorShower::AttackUnderEnemy()
{
	SpawnTime += TickRate;
	if (SpawnTime > SpawnDuration + SpawnHeight / FallingSpeed)
	{
		GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
		return;
	}

	if (!TargetCharacter.IsValid() || !CombatComponent.IsValid()) return;

	FVector TargetLocation = TargetCharacter->GetActorLocation();
	if (TargetLocation.Z < SpawnLocation.Z && TargetLocation.Z > SpawnLocation.Z - SpawnHeight * 2)
	{
		FVector2D SpawnVector2D = FVector2D(SpawnLocation.X, SpawnLocation.Y);
		FVector2D TargetLocation2D = FVector2D(TargetLocation.X, TargetLocation.Y);
		if (FVector2D::Distance(SpawnVector2D, TargetLocation2D) < 100.0f)
		{
			CombatComponent->ApplySkillDamage(TargetCharacter.Get(), CombatComponent->GetOwner(), SkillType);
		}
	}
	
}
