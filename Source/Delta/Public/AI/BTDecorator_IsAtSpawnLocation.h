// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_IsAtLocation.h"
#include "BTDecorator_IsAtSpawnLocation.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UBTDecorator_IsAtSpawnLocation : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsAtSpawnLocation();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, Category = "Values")
	float AcceptRadius = 10000.0f;
	
	UPROPERTY(EditAnywhere, Category = "Values")
	bool bCalcZAxis = false;

};
