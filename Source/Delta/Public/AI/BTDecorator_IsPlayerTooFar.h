// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/ValueOrBBKey.h"
#include "BTDecorator_IsPlayerTooFar.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UBTDecorator_IsPlayerTooFar : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsPlayerTooFar();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Values")
	FBlackboardKeySelector PlayerActorKey;
	
	UPROPERTY(EditAnywhere, Category = "Values")
	FValueOrBBKey_Float MaxDistance = 1000.0f;
	
};
