// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTTask_AllyTargetUpdate.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controllers/DeltaAllyController.h"

UBTTask_AllyTargetUpdate::UBTTask_AllyTargetUpdate()
{
	NodeName = "Choose 1 Target from enemies (Ally)";
}

EBTNodeResult::Type UBTTask_AllyTargetUpdate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ADeltaAllyController* OwnerAllyController = Cast<ADeltaAllyController>(OwnerComp.GetAIOwner());
	if (!OwnerAllyController) return EBTNodeResult::Failed;

	AActor* TargetActor = OwnerAllyController->GetRandEnemyTarget(MaxDistance);
	if (!TargetActor) return EBTNodeResult::Failed;

	if (!OwnerComp.GetBlackboardComponent()) return EBTNodeResult::Failed;

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetActorKey.SelectedKeyName, TargetActor);

	return EBTNodeResult::Succeeded;
}
