// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AllyTargetUpdate.generated.h"

/**
 * Behavior tree task for ally characters to find and target enemy characters
 */
UCLASS()
class DELTA_API UBTTask_AllyTargetUpdate : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_AllyTargetUpdate();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard Key")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Values")
	float MaxDistance = 1000.0f;
};
