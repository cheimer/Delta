// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_NeedCollectMana.h"

#include "AIController.h"
#include "Characters/DeltaBaseCharacter.h"

UBTDecorator_NeedCollectMana::UBTDecorator_NeedCollectMana()
{
	NodeName = "Not Enough Mana?";
}

bool UBTDecorator_NeedCollectMana::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	if (!IsValid(&OwnerComp) || !OwnerComp.GetAIOwner()) return false;

	ADeltaBaseCharacter* OwnerCharacter = Cast<ADeltaBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!OwnerCharacter) return false;

	return !OwnerCharacter->CanUseCurrentSkill();
}
