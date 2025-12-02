// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/MeshPoolSubsystem.h"

UMeshPoolSubsystem* UMeshPoolSubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull))
		{
			return World->GetSubsystem<UMeshPoolSubsystem>();
		}
	}

	return nullptr;
}

bool UMeshPoolSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	TArray<UClass*> FoundClasses;
	GetDerivedClasses(GetClass(), FoundClasses);
	
	return FoundClasses.IsEmpty();
}

AActor* UMeshPoolSubsystem::GetSkillMesh(TSubclassOf<AActor> SkillMeshClass)
{
	AActor* SkillMesh = nullptr;
	for (auto MeshActor : MeshPool.FindRef(SkillMeshClass))
	{
		if (MeshActor->IsHidden())
		{
			SkillMesh = MeshActor;
			break;
		}
	}

	if (!SkillMesh)
	{
		SkillMesh = GetWorld()->SpawnActor(SkillMeshClass);
		MeshPool.FindOrAdd(SkillMeshClass).Add(SkillMesh);
	}

	SkillMesh->SetActorHiddenInGame(false);
	
	return SkillMesh;
}

void UMeshPoolSubsystem::ReturnSkillMesh(AActor* ReturnMesh)
{
	if (!ReturnMesh) return;

	ReturnMesh->SetActorHiddenInGame(true);
	
}
