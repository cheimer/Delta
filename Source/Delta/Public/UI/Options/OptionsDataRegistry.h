// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OptionsDataRegistry.generated.h"

class UListDataObjectBase;
class UListDataObjectCollection;
/**
 * 
 */
UCLASS()
class DELTA_API UOptionsDataRegistry : public UObject
{
	GENERATED_BODY()

public:
	void InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer);

	const TArray<UListDataObjectCollection*>& GetRegisteredOptionsTabCollections() const {return RegisteredOptionsTabCollections;}

	TArray<UListDataObjectBase*> GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID) const;

private:
	void InitGameplayCollectionTab();
	void InitAudioCollectionTab();
	void InitVideoCollectionTab();
	void InitControlCollectionTab(ULocalPlayer* InOwningLocalPlayer);

	void FindChildListDataRecursively(UListDataObjectBase* InParentData, TArray<UListDataObjectBase*>& OutFoundChildListData) const;

	UPROPERTY(Transient)
	TArray<UListDataObjectCollection*> RegisteredOptionsTabCollections;
	
};
