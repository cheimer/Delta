// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataListEntryMappingDataAsset.generated.h"

class UListDataObjectBase;
class UListEntryWidgetBase;
/**
 * 
 */
UCLASS()
class DELTA_API UDataListEntryMappingDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	TSubclassOf<UListEntryWidgetBase> FindEntryWidgetClassByDataObject(UListDataObjectBase* InDataObject) const;

private:
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<UListDataObjectBase>, TSubclassOf<UListEntryWidgetBase>> DataObjectListEntryMap;
	
};
