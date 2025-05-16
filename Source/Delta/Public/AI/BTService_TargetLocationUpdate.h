// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_TargetLocationUpdate.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UBTService_TargetLocationUpdate : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_TargetLocationUpdate();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Blackboard Key")
	FBlackboardKeySelector TargetActorKey;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard Key")
	FBlackboardKeySelector TargetLocationKey;
	
};
