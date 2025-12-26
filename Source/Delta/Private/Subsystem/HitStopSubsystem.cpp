// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/HitStopSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "LevelInstance/LevelInstanceTypes.h"

UHitStopSubsystem* UHitStopSubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull))
		{
			return World->GetSubsystem<UHitStopSubsystem>();
		}
	}

	return nullptr;
}

bool UHitStopSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	TArray<UClass*> FoundClasses;
	GetDerivedClasses(GetClass(), FoundClasses);

	return FoundClasses.IsEmpty();
}

bool UHitStopSubsystem::StartHitStop(const AActor* OccurActor, const float MaxDuration, const float TimeDilation, const EHitStopPriority Priority)
{
	if (!GetWorld()) return false;
	
	if (Priority <= CurrentPriority)
	{
		CurrentPriority = Priority;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);

		GetWorld()->GetTimerManager().ClearTimer(HitStopTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(HitStopTimerHandle, this, &ThisClass::EndHitStopTimer, MaxDuration, false);

		RequestActors.Add(OccurActor);

		return true;
	}
	else
	{
		return false;
	}
	
}

void UHitStopSubsystem::EndHitStopTimer()
{
	if (!GetWorld()) return;
	
	for (auto It = RequestActors.CreateIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			It.RemoveCurrent();
		}
	}

	if (RequestActors.IsEmpty())
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

		CurrentPriority = EHitStopPriority::Default;
	}
}

void UHitStopSubsystem::EndHitStop(const AActor* OccurActor)
{
	if (!GetWorld()) return;

	if (RequestActors.Find(OccurActor))
	{
		RequestActors.Remove(OccurActor);
	}
	
	if (RequestActors.IsEmpty())
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

		CurrentPriority = EHitStopPriority::Default;
	}
}
