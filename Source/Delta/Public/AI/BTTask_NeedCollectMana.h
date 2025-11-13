// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_NeedCollectMana.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UBTTask_NeedCollectMana : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_NeedCollectMana();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
