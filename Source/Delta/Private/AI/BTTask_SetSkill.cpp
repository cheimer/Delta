// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_SetSkill.h"

#include "Controllers/DeltaAIController.h"

UBTTask_SetSkill::UBTTask_SetSkill()
{
	NodeName = "Set Skill";
}

EBTNodeResult::Type UBTTask_SetSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ADeltaAIController* OwnerController = Cast<ADeltaAIController>(OwnerComp.GetAIOwner());
	if (!OwnerController) return EBTNodeResult::Failed;

	OwnerController->SetCurrentSkill();

	return EBTNodeResult::Succeeded;
}
