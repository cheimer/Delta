// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/HitStopSubsystem.h"

#include "Kismet/GameplayStatics.h"

bool UHitStopSubsystem::StartHitStop(const float MaxDuration, const float TimeDilation, const EHitStopPriority Priority)
{
	if (!GetWorld()) return false;
	
	if (Priority <= CurrentPriority)
	{
		CurrentPriority = Priority;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);

		GetWorld()->GetTimerManager().ClearTimer(HitStopTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(HitStopTimerHandle, this, &ThisClass::EndHitStop, MaxDuration, false);

		return true;
	}
	else
	{
		return false;
	}
	
}

void UHitStopSubsystem::EndHitStop()
{
	if (!GetWorld()) return;
	
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}
