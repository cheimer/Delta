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
	
	AActor* SetRandTarget(const float MaxDistance = 1000.0f);

	void SetCurrentSkill();
	TOptional<float> GetCurrentSkillRange();
	float GetCurrentSkillDuration();
	
	void AttackTarget();

	void RotateToTarget();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree = nullptr;

	TWeakObjectPtr<ADeltaBaseCharacter> DeltaOwnerCharacter;
	TWeakObjectPtr<ADeltaBaseCharacter> CurrentTarget;
	UPROPERTY()
	TArray<TWeakObjectPtr<ADeltaBaseCharacter>> Targets;

public:
#pragma region GetSet
	ADeltaBaseCharacter* GetCurrentTarget() const {return CurrentTarget.IsValid() ? CurrentTarget.Get() : nullptr;}
	
#pragma endregion GetSet
	
};
