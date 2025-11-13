// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_IsTargetClose.h"

#include "Controllers/DeltaAIController.h"

UBTDecorator_IsTargetClose::UBTDecorator_IsTargetClose()
{
	NodeName = "Is Target Close?";
}

bool UBTDecorator_IsTargetClose::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	ADeltaAIController* OwnerController = Cast<ADeltaAIController>(OwnerComp.GetAIOwner());
	if (!OwnerController) return false;

	if (OwnerController->SetRandTarget(MaxDistance))
	{
		return true;
	}
	else
	{
		return false;
	}
	
}
