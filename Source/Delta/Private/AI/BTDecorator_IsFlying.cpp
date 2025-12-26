// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_IsFlying.h"

#include "AIController.h"
#include "Interfaces/Flying.h"

UBTDecorator_IsFlying::UBTDecorator_IsFlying()
{
	NodeName = "Checking Flying Interface";
}

bool UBTDecorator_IsFlying::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* OwnerPawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	if (!OwnerPawn) return false;

	IFlying* FlyingInterface = Cast<IFlying>(OwnerPawn);
	if (!FlyingInterface) return false;

	return FlyingInterface->IsFlying();
	
}
