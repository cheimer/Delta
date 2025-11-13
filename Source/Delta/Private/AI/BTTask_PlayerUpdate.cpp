// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_PlayerUpdate.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/DeltaBaseCharacter.h"
#include "Controllers/DeltaAllyController.h"

UBTTask_PlayerUpdate::UBTTask_PlayerUpdate()
{
	NodeName = "Find Playing Character";
}

EBTNodeResult::Type UBTTask_PlayerUpdate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ADeltaAllyController* OwnerAllyController = Cast<ADeltaAllyController>(OwnerComp.GetAIOwner());
	if (!OwnerAllyController) return EBTNodeResult::Failed;

	ADeltaBaseCharacter* PlayerLeader = OwnerAllyController->GetPlayingCharacter();
	if (!PlayerLeader) return EBTNodeResult::Failed;

	if (!OwnerComp.GetBlackboardComponent()) return EBTNodeResult::Failed;

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(PlayerActorKey.SelectedKeyName, PlayerLeader);

	return EBTNodeResult::Succeeded;
}
