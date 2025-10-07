// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Options/DataObject/ListDataObjectBase.h"
#include "ListDataObjectCollection.generated.h"

/**
 * 
 */
UCLASS()
class DELTA_API UListDataObjectCollection : public UListDataObjectBase
{
	GENERATED_BODY()

public:
	void AddChildListData(UListDataObjectBase* InChildListData);

	virtual TArray<UListDataObjectBase*> GetAllChildListData() const override;
	virtual bool HasAnyChildListData() const override;

private:
	UPROPERTY(Transient)
	TArray<UListDataObjectBase*> ChildListDataArray;
	
};
