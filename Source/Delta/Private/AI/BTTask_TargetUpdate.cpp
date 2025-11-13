// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TargetUpdate.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Controllers/DeltaAIController.h"

UBTTask_TargetUpdate::UBTTask_TargetUpdate()
{
	NodeName = "Choose 1 Rand Target in distance";
}

EBTNodeResult::Type UBTTask_TargetUpdate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ADeltaAIController* OwnerAIController = Cast<ADeltaAIController>(OwnerComp.GetAIOwner());
	if (!OwnerAIController) return EBTNodeResult::Failed;

	AActor* TargetActor = OwnerAIController->SetRandTarget(MaxDistance);
	if (!TargetActor) return EBTNodeResult::Failed;

	if (!OwnerComp.GetBlackboardComponent()) return EBTNodeResult::Failed;
	
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetActorKey.SelectedKeyName, TargetActor);

	return EBTNodeResult::Succeeded;
}
