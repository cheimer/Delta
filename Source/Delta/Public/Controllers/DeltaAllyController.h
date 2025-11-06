// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DeltaAllyController.generated.h"

class ADeltaBaseCharacter;
class ADeltaPlayableCharacter;

/**
 * DeltaAllyController - AI Controller for playable characters when not controlled by player
 * Handles AI behavior for team members including:
 * - Following the player-controlled character
 * - Attacking enemies
 * - Supporting team members
 */
UCLASS()
class DELTA_API ADeltaAllyController : public AAIController
{
	GENERATED_BODY()

public:
	ADeltaAllyController();

	// Find random enemy target within range
	AActor* GetRandEnemyTarget(const float MaxDistance = 1000.0f);

	// Find the player character to follow
	ADeltaPlayableCharacter* GetPlayerLeader();

	void SetCurrentSkill();
	TOptional<float> GetCurrentSkillRange();
	float GetCurrentSkillDuration();

	void AttackTarget();

protected:
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

private:
	TWeakObjectPtr<ADeltaPlayableCharacter> DeltaOwnerCharacter;
	TWeakObjectPtr<ADeltaBaseCharacter> CurrentEnemyTarget;

	UPROPERTY()
	TArray<ADeltaBaseCharacter*> EnemyTargets;

	FVector SpawnLocation;

public:
#pragma region GetSet

	ADeltaBaseCharacter* GetCurrentTarget() const { return CurrentEnemyTarget.IsValid() ? CurrentEnemyTarget.Get() : nullptr; }
	FVector GetSpawnLocation() const { return SpawnLocation; }

#pragma endregion GetSet
};
