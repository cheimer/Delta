// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "BTTask_WaitAttackDelay.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UBTTask_WaitAttackDelay : public UBTTask_Wait
{
	GENERATED_BODY()

public:
	UBTTask_WaitAttackDelay();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard Key")
	FBlackboardKeySelector AttackDelayTimeKey;
	
};
