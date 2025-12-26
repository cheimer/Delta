// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillMeteorShower.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Components/CombatComponent.h"
#include "Kismet/GameplayStatics.h"

USkillMeteorShower::USkillMeteorShower()
{
	SkillType = EDeltaSkillType::MeteorShower;
}

void USkillMeteorShower::BeginSkill(UCombatComponent* InCombatComponent)
{
	Super::BeginSkill(InCombatComponent);
	
	if (!MeteorShowerVFX)
	{
		UE_LOG(LogTemp, Warning, TEXT("MeteorShow VFX is not settings"));
		return;
	}
	
	if (!InCombatComponent || !InCombatComponent->GetOwner()) return;
	
	AActor* OwnerActor = InCombatComponent->GetOwner();
	SpawnLocation = InCombatComponent->GetSkillTargetLocation(false) + FVector::UpVector * SpawnHeight;
	SpawnLocation.Z = OwnerActor->GetActorLocation().Z + SpawnHeight;
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, MeteorShowerVFX, SpawnLocation, OwnerActor->GetActorRotation(), OwnerActor->GetActorScale(),
		true, true, ENCPoolMethod::AutoRelease, true);
	
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ThisClass::AttackUnderEnemy, TickRate, true, SpawnHeight / FallingSpeed);
	SpawnTime = SpawnHeight / FallingSpeed;
}

void USkillMeteorShower::AttackUnderEnemy()
{ 
	SpawnTime += TickRate;
	if (SpawnTime > SpawnDuration + SpawnHeight / FallingSpeed)
	{
		GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
		return;
	}
	
	if (!CombatComponent.IsValid() && !CombatComponent->GetOwner()) return;
	
	TArray<FHitResult> HitResults;
	FVector EndLocation = SpawnLocation + -FVector::UpVector * SpawnHeight * 1.5f;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	CombatComponent->GetTargetTraceChannel(ObjectTypes);

	UKismetSystemLibrary::BoxTraceMultiForObjects(this, SpawnLocation, EndLocation, FVector(1.0f, 100.0f, 100.0f),
		(-FVector::UpVector).ToOrientationRotator(), ObjectTypes, false,TArray<AActor*>(),
		EDrawDebugTrace::None, HitResults, true);

	
	for (auto HitResult : HitResults)
	{
		if (!HitResult.GetActor()) continue;

		if (CombatComponent->GetIsOpponent(HitResult.GetActor()).Get(false))
		{
			CombatComponent->ApplySkillDamage(HitResult.GetActor(), CombatComponent->GetOwner(), SkillDamage);
			return; // Hit only first target
		}
	}
	
}
