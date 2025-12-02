// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeltaTypes/DeltaEnumTypes.h"
#include "Subsystems/WorldSubsystem.h"
#include "MeshPoolSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UMeshPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	static UMeshPoolSubsystem* Get(const UObject* WorldContextObject);
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	AActor* GetSkillMesh(TSubclassOf<AActor> SkillMeshClass);
	void ReturnSkillMesh(AActor* ReturnMesh);

private:
	TMap<UClass*, TArray<AActor*>> MeshPool;
	
};
