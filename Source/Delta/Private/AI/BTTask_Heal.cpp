// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Heal.h"

#include "Components/HealthComponent.h"
#include "Controllers/DeltaAIController.h"

UBTTask_Heal::UBTTask_Heal()
{
	NodeName = "Healing";
}

EBTNodeResult::Type UBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* OwnerPawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	if (!OwnerPawn) return EBTNodeResult::Failed;

	UHealthComponent* OwnerHealth = OwnerPawn->GetComponentByClass<UHealthComponent>();
	if (!OwnerHealth) return EBTNodeResult::Failed;

	OwnerHealth->SetHealthPercentage(HealToRatio);
	return EBTNodeResult::Succeeded;
	
}
