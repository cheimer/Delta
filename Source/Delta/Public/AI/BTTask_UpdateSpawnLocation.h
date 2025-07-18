// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_UpdateSpawnLocation.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UBTTask_UpdateSpawnLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_UpdateSpawnLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard Key")
	FBlackboardKeySelector SpawnLocationKey;

};
