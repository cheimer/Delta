// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AttackTarget.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UBTTask_AttackTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_AttackTarget();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

	UFUNCTION()
	void HandleCurrentMontageEnd(UAnimMontage* Montage, bool bInterrupted);

private:
	void CleanUpTask();
	
	UPROPERTY(EditAnywhere, Category = "Blackboard Key")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY()
	UBehaviorTreeComponent* CurrentOwnerComp;
	
};
