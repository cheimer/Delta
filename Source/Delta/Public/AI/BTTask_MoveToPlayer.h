// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToPlayer.generated.h"

/**
 * Moves ally to player-controlled character's location
 * Maintains a specific distance range from player
 */
UCLASS()
class DELTA_API UBTTask_MoveToPlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_MoveToPlayer();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	// Desired distance from player (ally will stop at this distance)
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0.0"))
	float AcceptanceRadius = 300.0f;

	// Movement speed multiplier when returning to player
	// Use higher value (>1.0) for urgency when too far
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0.5", ClampMax = "2.0"))
	float SpeedMultiplier = 1.0f;

	// If true, uses faster movement when far from player
	UPROPERTY(EditAnywhere, Category = "Movement")
	bool bUseUrgentSpeed = true;
};
