// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsPlayableClose.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UBTDecorator_IsPlayableClose : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsPlayableClose();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	UPROPERTY(EditAnywhere, Category = "Values")
	float MaxDistance = 10000.0f;
	
};
