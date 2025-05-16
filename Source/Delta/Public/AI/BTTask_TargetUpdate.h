// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TargetUpdate.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UBTTask_TargetUpdate : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_TargetUpdate();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard Key")
	FBlackboardKeySelector TargetActorKey;
	
};
