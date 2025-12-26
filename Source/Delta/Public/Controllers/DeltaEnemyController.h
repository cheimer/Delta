// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controllers/DeltaAIController.h"
#include "DeltaEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API ADeltaEnemyController : public ADeltaAIController
{
	GENERATED_BODY()

	
protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	FVector SpawnLocation;

public:
#pragma region GetSet
	FVector GetSpawnLocation() const { return SpawnLocation; }
	
#pragma endregion GetSet
	
};
