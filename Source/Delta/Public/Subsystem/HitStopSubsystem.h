// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "HitStopSubsystem.generated.h"

UENUM()
enum class EHitStopPriority
{
	Player = 0,
	Enemy = 1,
	
	Default = 99,
};

/**
 * 
 */
UCLASS()
class DELTA_API UHitStopSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	bool StartHitStop(const float MaxDuration, const float TimeDilation, const EHitStopPriority Priority = EHitStopPriority::Default);
	void EndHitStop();

private:
	EHitStopPriority CurrentPriority = EHitStopPriority::Default;

	FTimerHandle HitStopTimerHandle;
	
};
