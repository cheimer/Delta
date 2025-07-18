// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DeltaAIController.generated.h"

class ADeltaBaseCharacter;
class ADeltaEnemyCharacter;
class UAIPerceptionComponent;

/**
 * 
 */
UCLASS()
class DELTA_API ADeltaAIController : public AAIController
{
	GENERATED_BODY()

public:
	ADeltaAIController();
	
	AActor* GetRandPlayableTarget(const float MaxDistance = 1000.0f);

	void SetCurrentSkill();
	TOptional<float> GetCurrentSkillRange();
	void AttackTarget();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

private:
	TWeakObjectPtr<ADeltaEnemyCharacter> DeltaOwnerCharacter;
	TWeakObjectPtr<ADeltaBaseCharacter> CurrentPlayableTarget;
	UPROPERTY()
	TArray<ADeltaBaseCharacter*> PlayableTargets;

	FVector SpawnLocation;

public:
#pragma region GetSet
	ADeltaBaseCharacter* GetCurrentTarget() const {return CurrentPlayableTarget.IsValid() ? CurrentPlayableTarget.Get() : nullptr;}
	FVector GetSpawnLocation() const { return SpawnLocation; }
	
#pragma endregion GetSet
	
};
