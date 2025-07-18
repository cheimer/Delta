// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_MoveToDynamicRadius.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UBTTask_MoveToDynamicRadius : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:
	UBTTask_MoveToDynamicRadius();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
