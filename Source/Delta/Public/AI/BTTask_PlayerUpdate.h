// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PlayerUpdate.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UBTTask_PlayerUpdate : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_PlayerUpdate();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard Key")
	FBlackboardKeySelector PlayerActorKey;
	
};
