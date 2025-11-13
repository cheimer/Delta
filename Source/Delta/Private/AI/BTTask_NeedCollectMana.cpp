// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_NeedCollectMana.h"

#include "AIController.h"
#include "Characters/DeltaBaseCharacter.h"

UBTTask_NeedCollectMana::UBTTask_NeedCollectMana()
{
	NodeName = "Not Enough Mana?";
}

EBTNodeResult::Type UBTTask_NeedCollectMana::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!IsValid(&OwnerComp) || !OwnerComp.GetAIOwner()) return EBTNodeResult::Failed;

	ADeltaBaseCharacter* OwnerCharacter = Cast<ADeltaBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!OwnerCharacter) return EBTNodeResult::Failed;

	return !OwnerCharacter->CanUseCurrentSkill() ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
