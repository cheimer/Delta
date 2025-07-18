// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetSkill.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UBTTask_SetSkill : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_SetSkill();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
