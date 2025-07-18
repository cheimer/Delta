// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_IsPlayableClose.h"

#include "Controllers/DeltaAIController.h"

UBTDecorator_IsPlayableClose::UBTDecorator_IsPlayableClose()
{
	NodeName = "Is Playable Close?";
}

bool UBTDecorator_IsPlayableClose::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	ADeltaAIController* OwnerController = Cast<ADeltaAIController>(OwnerComp.GetAIOwner());
	if (!OwnerController) return false;

	if (OwnerController->GetRandPlayableTarget(MaxDistance))
	{
		return true;
	}
	else
	{
		return false;
	}
	
}
