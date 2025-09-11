// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillForestBoom.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/DeltaBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/CombatComponent.h"
#include "Kismet/GameplayStatics.h"

USkillForestBoom::USkillForestBoom()
{
	SkillType = EDeltaSkillType::ForestBoom;
}

void USkillForestBoom::BeginSkill(UCombatComponent* InCombatComponent)
{
	Super::BeginSkill(InCombatComponent);
	
	float SpawnDelay = SpawnDuration / SpawnNum;

	GetWorld()->GetTimerManager().SetTimer(SpawnGroundTimer, this, &ThisClass::SpawnGroundVFX, SpawnDelay, true, 0.0f);
	
}

void USkillForestBoom::EndSkill()
{
	Super::EndSkill();
	
	float SpawnDelay = SpawnDuration / SpawnNum;

	GetWorld()->GetTimerManager().SetTimer(SpawnBoomTimer, this, &ThisClass::SpawnBoomVFX, SpawnDelay, true, 0.0f);
	
}

void USkillForestBoom::SpawnGroundVFX()
{
	if (!GroundVFX || !CombatComponent.IsValid() || !CombatComponent->GetSkillTargetActor()|| SpawnedGroundNiagaras.Num() >= SpawnNum)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnGroundTimer);
		return;
	}

	FVector SpawnLocation;
	if (SpawnedGroundNiagaras.Num() == 0)
	{
		SpawnLocation = CombatComponent->GetSkillTargetActor()->GetActorLocation();
	}
	else
	{
		FVector CurrentLocation = CombatComponent->GetSkillTargetActor()->GetActorLocation();
		FVector BeforeLocation = TargetLocations.Last();
		FVector ForwardVector = CurrentLocation - BeforeLocation;

		SpawnLocation = CurrentLocation + (ForwardVector * 1.5f);
	}

	if (CombatComponent->GetSkillTargetActor()->GetCapsuleComponent())
	{
		float CapsuleRadius, CapsuleHeight;
		CombatComponent->GetSkillTargetActor()->GetCapsuleComponent()->GetScaledCapsuleSize(CapsuleRadius, CapsuleHeight);
		
		SpawnLocation.Z -= CapsuleHeight;
		SpawnLocation.Z += 5.0f;
	}
	
	TargetLocations.Add(CombatComponent->GetSkillTargetActor()->GetActorLocation());
	
	if (UNiagaraComponent* SpawnNiagara = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GroundVFX, SpawnLocation, FRotator::ZeroRotator))
	{
		SpawnNiagara->SetRelativeScale3D(FVector(AOERadius / DefaultRadius));
		SpawnedGroundNiagaras.Add(SpawnNiagara);
	}
	
}

void USkillForestBoom::SpawnBoomVFX()
{
	if (!BoomVFX || !CombatComponent.IsValid() || !CombatComponent->GetOwner() ||
		SpawnBoomIndex >= SpawnedGroundNiagaras.Num() || !SpawnedGroundNiagaras.IsValidIndex(SpawnBoomIndex))
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnBoomTimer);
		return;
	}

	FVector SpawnLocation = SpawnedGroundNiagaras[SpawnBoomIndex]->GetComponentLocation();

	if (UNiagaraComponent* SpawnNiagara = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BoomVFX, SpawnLocation, FRotator::ZeroRotator))
	{
		SpawnNiagara->SetRelativeScale3D(FVector(AOERadius / DefaultRadius));
	}
	
	SpawnedGroundNiagaras[SpawnBoomIndex]->DestroyInstance();

	TArray<AActor*> HitActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	CombatComponent->GetTargetTraceChannel(ObjectTypes);
	
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), SpawnLocation, AOERadius,
		ObjectTypes, ADeltaBaseCharacter::StaticClass(), TArray<AActor*>(), HitActors);
	
	for (auto HitActor : HitActors)
	{
		if (!HitActor) continue;

		if (CombatComponent->GetIsOpponent(HitActor).Get(false))
		{
			CombatComponent->ApplySkillDamage(HitActor, CombatComponent->GetOwner(), SkillDamage);
		}
	}

	SpawnBoomIndex++;
}
