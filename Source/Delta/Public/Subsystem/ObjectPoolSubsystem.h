// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ObjectPoolSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UObjectPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	static UObjectPoolSubsystem* Get(const UObject* WorldContextObject);
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	AActor* GetObject(UClass* ObjectClass);
	void ReturnObject(AActor* ReturnObjectActor);

private:
	TMap<UClass*, TArray<AActor*>> ObjectPool;
	
};
