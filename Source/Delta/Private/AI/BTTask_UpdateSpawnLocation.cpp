// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_UpdateSpawnLocation.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Controllers/DeltaEnemyController.h"

UBTTask_UpdateSpawnLocation::UBTTask_UpdateSpawnLocation()
{
	NodeName = "Update Spawn Location from Owner";
}

EBTNodeResult::Type UBTTask_UpdateSpawnLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ADeltaEnemyController* OwnerController = Cast<ADeltaEnemyController>(OwnerComp.GetAIOwner());
	if (!OwnerController) return EBTNodeResult::Failed;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;
	
	FVector SpawnLocation = OwnerController->GetSpawnLocation();

	Blackboard->SetValueAsVector(SpawnLocationKey.SelectedKeyName, SpawnLocation);
	return EBTNodeResult::Succeeded;
	
}
