// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/DeltaEnemyController.h"

void ADeltaEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	SpawnLocation = InPawn->GetActorLocation();
	
}
