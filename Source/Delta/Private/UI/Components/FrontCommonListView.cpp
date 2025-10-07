// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Components/FrontCommonListView.h"

#include "DataAssets/UI/DataListEntryMappingDataAsset.h"
#include "Editor/WidgetCompilerLog.h"
#include "UI/Options/DataObject/ListDataObjectBase.h"
#include "UI/Options/DataObject/ListDataObjectCollection.h"
#include "UI/Options/ListEntries/ListEntryWidgetBase.h"

#if WITH_EDITOR	
void UFrontCommonListView::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!DataListEntryMapping)
	{
		CompileLog.Error(FText::FromString("DA_DataListEntryMapping not set"));
	}
}
#endif

UUserWidget& UFrontCommonListView::OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (IsDesignTime())
	{
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}
	else
	{
		if (TSubclassOf<UListEntryWidgetBase> FoundWidgetClass = DataListEntryMapping->FindEntryWidgetClassByDataObject(CastChecked<UListDataObjectBase>(Item)))
		{
			return GenerateTypedEntry<UListEntryWidgetBase>(FoundWidgetClass, OwnerTable);
		}
		else
		{
			return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
		}
	}
}

bool UFrontCommonListView::OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem)
{
	bool SuperValue = Super::OnIsSelectableOrNavigableInternal(FirstSelectedItem);

	if (FirstSelectedItem->IsA<UListDataObjectCollection>())
	{
		return false;
	}
	
	return SuperValue;
}
