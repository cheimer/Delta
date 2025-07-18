// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateAttackLocation.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UBTService_UpdateAttackLocation : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_UpdateAttackLocation();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackbaord")
	FBlackboardKeySelector AttackLocation;
	
};
