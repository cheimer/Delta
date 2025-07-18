// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsFlying.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UBTDecorator_IsFlying : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsFlying();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
