// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_MoveToDynamicRadius.h"

#include "Controllers/DeltaAIController.h"

UBTTask_MoveToDynamicRadius::UBTTask_MoveToDynamicRadius()
{
	NodeName = "Move To Acceptable Range Skill Distance";

}

EBTNodeResult::Type UBTTask_MoveToDynamicRadius::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ADeltaAIController* OwnerController = Cast<ADeltaAIController>(OwnerComp.GetAIOwner());
	if (!OwnerController) return EBTNodeResult::Failed;

	TOptional<float> SkillRange = OwnerController->GetCurrentSkillRange();
	if (!SkillRange.IsSet()) return EBTNodeResult::Failed;
	
	AcceptableRadius = SkillRange.GetValue();
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
