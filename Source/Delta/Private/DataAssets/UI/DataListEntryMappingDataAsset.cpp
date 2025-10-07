// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/UI/DataListEntryMappingDataAsset.h"

#include "UI/Options/DataObject/ListDataObjectBase.h"

TSubclassOf<UListEntryWidgetBase> UDataListEntryMappingDataAsset::FindEntryWidgetClassByDataObject(UListDataObjectBase* InDataObject) const
{
	check(InDataObject);

	for(UClass* DataObjectClass = InDataObject->GetClass(); DataObjectClass; DataObjectClass = DataObjectClass->GetSuperClass())
	{
		if (TSubclassOf<UListDataObjectBase> ConvertedDataObjectClass = TSubclassOf<UListDataObjectBase>(DataObjectClass))
		{
			if (DataObjectListEntryMap.Contains(ConvertedDataObjectClass))
			{
				return DataObjectListEntryMap.FindRef(ConvertedDataObjectClass);
			}
		}
	}

	return TSubclassOf<UListEntryWidgetBase>();
}
