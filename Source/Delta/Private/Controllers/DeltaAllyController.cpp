// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/DeltaAllyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Characters/DeltaAllyCharacter.h"
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
		DeltaOwnerCharacter = DeltaOwnerCharacter.Get() ? DeltaOwnerCharacter.Get() : Cast<ADeltaAllyCharacter>(InPawn);
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
	DeltaOwnerCharacter = DeltaOwnerCharacter.IsValid() ? DeltaOwnerCharacter.Get() : Cast<ADeltaAllyCharacter>(GetPawn());
	if (!DeltaOwnerCharacter.IsValid()) return;

	// For now, allies will use the same skill selection as enemies
	// This can be extended later for more sophisticated ally behavior
	if (ADeltaAllyCharacter* AllyCharacter = Cast<ADeltaAllyCharacter>(DeltaOwnerCharacter.Get()))
	{
		// Implement skill selection logic here
		// For example, select based on distance to target, situation, etc.
	}
}

TOptional<float> ADeltaAllyController::GetCurrentSkillRange()
{
	DeltaOwnerCharacter = DeltaOwnerCharacter.IsValid() ? DeltaOwnerCharacter.Get() : Cast<ADeltaAllyCharacter>(GetPawn());
	if (!DeltaOwnerCharacter.IsValid()) return TOptional<float>();

	return DeltaOwnerCharacter->GetCurrentSkillRange();
}

float ADeltaAllyController::GetCurrentSkillDuration()
{
	DeltaOwnerCharacter = DeltaOwnerCharacter.IsValid() ? DeltaOwnerCharacter.Get() : Cast<ADeltaAllyCharacter>(GetPawn());
	if (!DeltaOwnerCharacter.IsValid()) return 0.0f;

	// For allies, we'll need to add skill tracking similar to enemies
	// This will depend on implementing SetCurrentSkill properly
	return 0.0f;
}

void ADeltaAllyController::AttackTarget()
{
	DeltaOwnerCharacter = DeltaOwnerCharacter.IsValid() ? DeltaOwnerCharacter.Get() : Cast<ADeltaAllyCharacter>(GetPawn());
	if (!DeltaOwnerCharacter.IsValid()) return;

	// For now, allies will need skill selection implemented
	// This can use the existing skill system from DeltaBaseCharacter
}
