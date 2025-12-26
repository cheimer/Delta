// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_WaitAttackDelay.h"

UBTTask_WaitAttackDelay::UBTTask_WaitAttackDelay()
{
	NodeName = "Wait with AttackDelay";
	WaitTime = 5.0f;
	bTickIntervals = true;
}

EBTNodeResult::Type UBTTask_WaitAttackDelay::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!IsValid(&OwnerComp) || !OwnerComp.GetBlackboardComponent()) return EBTNodeResult::Failed;
	const float AttackDelayTime = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(AttackDelayTimeKey.SelectedKeyName);
	
	const float WaitSecond = WaitTime.GetValue(OwnerComp) + AttackDelayTime;
	const float DeviationSecond = RandomDeviation.GetValue(OwnerComp); 
	const float RemainingWaitTime = FMath::FRandRange(FMath::Max(0.0f, WaitSecond - DeviationSecond), (WaitSecond + DeviationSecond));
	SetNextTickTime(NodeMemory, RemainingWaitTime);
	
	return EBTNodeResult::InProgress;
}
