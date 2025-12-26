// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_IsAtSpawnLocation.h"

#include "AIController.h"
#include "Controllers/DeltaEnemyController.h"

UBTDecorator_IsAtSpawnLocation::UBTDecorator_IsAtSpawnLocation()
{
	NodeName = "Is close Spawn Location?";

	FlowAbortMode = EBTFlowAbortMode::LowerPriority;

}

bool UBTDecorator_IsAtSpawnLocation::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	const ADeltaEnemyController* OwnerController = Cast<ADeltaEnemyController>(OwnerComp.GetAIOwner());
	if(!OwnerController) return false;

	const APawn* OwnerCharacter = OwnerComp.GetAIOwner()->GetPawn();
	if (!OwnerCharacter) return false;

	FVector OwnerLocation = OwnerCharacter->GetActorLocation();
	FVector SpawnLocation = OwnerController->GetSpawnLocation();

	if (!bCalcZAxis)
	{
		OwnerLocation.Z = 0.0f;
		SpawnLocation.Z = 0.0f;
	}

	if (FVector::Distance(OwnerLocation, SpawnLocation) < AcceptRadius)
	{
		return true;
	}
	else
	{
		return false;
	}
}
