// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_AttackTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Controllers/DeltaAIController.h"

UBTTask_AttackTarget::UBTTask_AttackTarget()
{
	NodeName = "Attack Target Actor";
}

EBTNodeResult::Type UBTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName);
	if (!TargetObject) return EBTNodeResult::Failed;
\
	ADeltaAIController* OwnerAIController = Cast<ADeltaAIController>(OwnerComp.GetAIOwner());
	if (!OwnerAIController) return EBTNodeResult::Failed;

	OwnerAIController->AttackTarget();
	
	return EBTNodeResult::Succeeded;
}
