// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Heal.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UBTTask_Heal : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Heal();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Values", meta = (ClampMin = 0.0, ClampMax = 1.0))
	float HealToRatio = 1.0f;
	
};
