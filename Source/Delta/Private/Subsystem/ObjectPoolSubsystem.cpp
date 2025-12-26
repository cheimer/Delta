// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/ObjectPoolSubsystem.h"

UObjectPoolSubsystem* UObjectPoolSubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull))
		{
			return World->GetSubsystem<UObjectPoolSubsystem>();
		}
	}

	return nullptr;
}

bool UObjectPoolSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	TArray<UClass*> FoundClasses;
	GetDerivedClasses(GetClass(), FoundClasses);
	
	return FoundClasses.IsEmpty();
}

AActor* UObjectPoolSubsystem::GetObject(UClass* ObjectClass)
{
	AActor* CurrentActor = nullptr;
	for (auto ActorFromPool : ObjectPool.FindRef(ObjectClass))
	{
		if (ActorFromPool->IsHidden())
		{
			CurrentActor = ActorFromPool;
			break;
		}
	}

	if (!CurrentActor)
	{
		CurrentActor = GetWorld()->SpawnActor(ObjectClass);
		ObjectPool.FindOrAdd(ObjectClass).Add(CurrentActor);
	}

	CurrentActor->SetActorHiddenInGame(false);
	
	return CurrentActor;
}

void UObjectPoolSubsystem::ReturnObject(AActor* ReturnObjectActor)
{
	if (!ReturnObjectActor) return;

	ReturnObjectActor->SetActorHiddenInGame(true);
	
}
