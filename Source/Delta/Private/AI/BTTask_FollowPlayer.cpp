// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTTask_FollowPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controllers/DeltaAllyController.h"
#include "Characters/DeltaPlayableCharacter.h"

UBTTask_FollowPlayer::UBTTask_FollowPlayer()
{
	NodeName = "Follow Player Leader";
}

EBTNodeResult::Type UBTTask_FollowPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ADeltaAllyController* OwnerAllyController = Cast<ADeltaAllyController>(OwnerComp.GetAIOwner());
	if (!OwnerAllyController) return EBTNodeResult::Failed;

	ADeltaPlayableCharacter* PlayerLeader = OwnerAllyController->GetPlayerLeader();
	if (!PlayerLeader) return EBTNodeResult::Failed;

	if (!OwnerComp.GetBlackboardComponent()) return EBTNodeResult::Failed;

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(PlayerActorKey.SelectedKeyName, PlayerLeader);

	return EBTNodeResult::Succeeded;
}
