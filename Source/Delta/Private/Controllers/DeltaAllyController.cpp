// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/DeltaAllyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Characters/DeltaPlayableCharacter.h"
#include "Characters/Enemy/DeltaEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

ADeltaAllyController::ADeltaAllyController()
{
}

void ADeltaAllyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	check(BehaviorTree && BehaviorTree->BlackboardAsset);
	if (BehaviorTree && BehaviorTree->BlackboardAsset)
	{
		DeltaOwnerCharacter = DeltaOwnerCharacter.Get() ? DeltaOwnerCharacter.Get() : Cast<ADeltaPlayableCharacter>(InPawn);
		if (!DeltaOwnerCharacter.IsValid()) return;

		RunBehaviorTree(BehaviorTree);
	}

	SpawnLocation = InPawn->GetActorLocation();
}

AActor* ADeltaAllyController::GetRandEnemyTarget(const float MaxDistance)
{
	if (EnemyTargets.IsEmpty())
	{
		TArray<AActor*> TempArray;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADeltaEnemyCharacter::StaticClass(), TempArray);

		for (AActor* CachedActor : TempArray)
		{
			if (ADeltaBaseCharacter* EnemyCharacter = Cast<ADeltaBaseCharacter>(CachedActor))
			{
				// Only add if it's an opponent (different team)
				if (!EnemyCharacter->GetIsDead() &&
					EnemyCharacter->GetTeamAffiliation() == ETeamAffiliation::Enemy)
				{
					EnemyTargets.Add(EnemyCharacter);
				}
			}
		}
	}

	// Clean up dead targets
	EnemyTargets.RemoveAll([](ADeltaBaseCharacter* Target) {
		return !Target || Target->GetIsDead();
	});

	if (EnemyTargets.IsEmpty()) return nullptr;

	if (!GetPawn()) return nullptr;

	TArray<ADeltaBaseCharacter*> TempArray;
	for (ADeltaBaseCharacter* CachedActor : EnemyTargets)
	{
		if (!CachedActor) continue;

		if (FVector::Distance(CachedActor->GetActorLocation(), GetPawn()->GetActorLocation()) < MaxDistance)
		{
			TempArray.Add(CachedActor);
		}
	}

	if (TempArray.IsEmpty()) return nullptr;

	int RandIndex = FMath::RandRange(0, TempArray.Num() - 1);

	CurrentEnemyTarget = TempArray[RandIndex];
	return CurrentEnemyTarget.Get();
}

ADeltaPlayableCharacter* ADeltaAllyController::GetPlayerLeader()
{
	TArray<AActor*> FoundPlayers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADeltaPlayableCharacter::StaticClass(), FoundPlayers);

	if (FoundPlayers.Num() > 0)
	{
		// Return the first player character found (main player)
		return Cast<ADeltaPlayableCharacter>(FoundPlayers[0]);
	}

	return nullptr;
}

void ADeltaAllyController::SetCurrentSkill()
{
	DeltaOwnerCharacter = DeltaOwnerCharacter.IsValid() ? DeltaOwnerCharacter.Get() : Cast<ADeltaPlayableCharacter>(GetPawn());
	if (!DeltaOwnerCharacter.IsValid()) return;

	// Select random skill from character's skill set
	DeltaOwnerCharacter->SetRandomSkill();
}

TOptional<float> ADeltaAllyController::GetCurrentSkillRange()
{
	DeltaOwnerCharacter = DeltaOwnerCharacter.IsValid() ? DeltaOwnerCharacter.Get() : Cast<ADeltaPlayableCharacter>(GetPawn());
	if (!DeltaOwnerCharacter.IsValid()) return TOptional<float>();

	return DeltaOwnerCharacter->GetCurrentSkillRange();
}

float ADeltaAllyController::GetCurrentSkillDuration()
{
	DeltaOwnerCharacter = DeltaOwnerCharacter.IsValid() ? DeltaOwnerCharacter.Get() : Cast<ADeltaPlayableCharacter>(GetPawn());
	if (!DeltaOwnerCharacter.IsValid()) return 0.0f;

	EDeltaSkillType CurrentSkill = DeltaOwnerCharacter->GetCurrentSkill();
	if (CurrentSkill == EDeltaSkillType::Max) return 0.0f;

	return DeltaOwnerCharacter->GetSkillDurationTime(CurrentSkill);
}

void ADeltaAllyController::AttackTarget()
{
	DeltaOwnerCharacter = DeltaOwnerCharacter.IsValid() ? DeltaOwnerCharacter.Get() : Cast<ADeltaPlayableCharacter>(GetPawn());
	if (!DeltaOwnerCharacter.IsValid()) return;

	EDeltaSkillType CurrentSkill = DeltaOwnerCharacter->GetCurrentSkill();
	if (CurrentSkill == EDeltaSkillType::Max) return;

	DeltaOwnerCharacter->PlaySkillAnimation(CurrentSkill);
}
