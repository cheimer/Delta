// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_NeedCollectMana.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UBTDecorator_NeedCollectMana : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_NeedCollectMana();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
