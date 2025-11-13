// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/ValueOrBBKey.h"
#include "BTTask_SetLocationFromTarget.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UBTTask_SetLocationFromTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_SetLocationFromTarget();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "BB Key")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "BB Key")
	FBlackboardKeySelector DestLocationKey;

	UPROPERTY(EditAnywhere, Category = "BB Key")
	FValueOrBBKey_Float LengthKey;

	UPROPERTY(EditAnywhere, Category = "Values", meta = (ClampMax = 2.0f, ClampMin = 1.0f))
	FValueOrBBKey_Float MaxMulti = 1.5f;
	
	UPROPERTY(EditAnywhere, Category = "Values", meta = (ClampMax = 1.0f, ClampMin = 0.0f))
	FValueOrBBKey_Float MinMulti = 0.5f;
	
	UPROPERTY(EditAnywhere, Category = "Values", meta = (ClampMax = 36, ClampMin = 0))
	FValueOrBBKey_Int32 PointNum = 12;
	
};
