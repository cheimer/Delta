// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckPlayerDistance.generated.h"

/**
 * Decorator that checks if ally is too far from player
 * Used to prioritize returning to player over combat
 */
UCLASS()
class DELTA_API UBTDecorator_CheckPlayerDistance : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CheckPlayerDistance();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	// Maximum distance from player before returning
	UPROPERTY(EditAnywhere, Category = "Distance", meta = (ClampMin = "0.0"))
	float MaxDistanceFromPlayer = 1500.0f;

	// If true, passes when distance > max (too far)
	// If false, passes when distance < max (close enough)
	UPROPERTY(EditAnywhere, Category = "Distance")
	bool bCheckIfTooFar = true;
};
