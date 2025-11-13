// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_IsPlayerTooFar.h"

#include "Characters/DeltaBaseCharacter.h"
#include "Controllers/DeltaAllyController.h"

UBTDecorator_IsPlayerTooFar::UBTDecorator_IsPlayerTooFar()
{
	NodeName = "Check Distance Owner to Player";
}

bool UBTDecorator_IsPlayerTooFar::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	if (!IsValid(&OwnerComp) || !OwnerComp.GetBlackboardComponent()) return false;

	ADeltaAllyController* AllyController = Cast<ADeltaAllyController>(OwnerComp.GetAIOwner());
	if (!AllyController) return false;

	APawn* OwnerPawn = AllyController->GetPawn();
	if (!OwnerPawn) return false;

	AActor* PlayerActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(PlayerActorKey.SelectedKeyName));
	if (!PlayerActor) return false;

	return FVector::Distance(OwnerPawn->GetActorLocation(), PlayerActor->GetActorLocation()) > MaxDistance.GetValue(OwnerComp);
}
