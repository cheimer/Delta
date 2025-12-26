// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToLocation.generated.h"

USTRUCT()
struct FMoveInfoTaskMemory
{
	GENERATED_BODY()
	
	TWeakObjectPtr<APawn> OwningPawn;
	TWeakObjectPtr<AActor> TargetActor;

	FVector DestinationLocation;
	FVector RelativeOffset;

	float LastUpdateTime;
	float UpdateInterval;

	FMoveInfoTaskMemory()
	{
		OwningPawn = nullptr;
		TargetActor = nullptr;
		
		DestinationLocation = FVector::ZeroVector;
		RelativeOffset = FVector::ZeroVector;
		
		LastUpdateTime = 0.0f;
		UpdateInterval = 0.5f;
	}
	
	bool IsValid() const
	{
		return OwningPawn.IsValid() && TargetActor.IsValid();
	}

	void Reset()
	{
		OwningPawn.Reset();
		TargetActor.Reset();
	}
	
};
 
/**
 * 
 */
UCLASS()
class DELTA_API UBTTask_MoveToLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_MoveToLocation();
	
protected:
	virtual uint16 GetInstanceMemorySize() const override;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Values")
	float AcceptRadius = 150.0f;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard Key")
	FBlackboardKeySelector TargetActorKey;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard Key")
	FBlackboardKeySelector LocationKey;
	
};
