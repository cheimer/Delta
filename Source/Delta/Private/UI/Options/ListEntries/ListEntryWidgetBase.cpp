// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Options/ListEntries/ListEntryWidgetBase.h"

#include "CommonInputSubsystem.h"
#include "CommonTextBlock.h"
#include "Components/ListView.h"
#include "UI/Options/DataObject/ListDataObjectBase.h"

void UListEntryWidgetBase::NativeOnListEntryWidgetHovered(bool bWasHovered)
{
	BP_OnListEntryWidgetHovered(bWasHovered, GetListItem() ? IsListItemSelected() : false);

	if (bWasHovered)
	{
		BP_OnToggleEntryWidgetHighlightState(true);
	}
	else
	{
		BP_OnToggleEntryWidgetHighlightState(GetListItem() && IsListItemSelected());
	}
}

void UListEntryWidgetBase::NativeOnListItemObjectSet(UObject* ListItemObject) 
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	OnOwningListDataObjectSet(CastChecked<UListDataObjectBase>(ListItemObject));
	
}

void UListEntryWidgetBase::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);

	BP_OnToggleEntryWidgetHighlightState(bIsSelected);
}

void UListEntryWidgetBase::NativeOnEntryReleased()
{
	IUserObjectListEntry::NativeOnEntryReleased();

	NativeOnListEntryWidgetHovered(false);
}

FReply UListEntryWidgetBase::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	FReply SuperValue = Super::NativeOnFocusReceived(InGeometry, InFocusEvent);

	UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();

	if (CommonInputSubsystem && CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		if(UWidget* WidgetToFocus = BP_GetWidgetToFocusForGamepad())
		{
			if (TSharedPtr<SWidget> SlateWidgetToFocus = WidgetToFocus->GetCachedWidget())
			{
				return FReply::Handled().SetUserFocus(SlateWidgetToFocus.ToSharedRef());
			}
		}
	}

	return SuperValue;
	
}

void UListEntryWidgetBase::OnOwningListDataObjectSet(UListDataObjectBase* InOwningListDataObject)
{
	if (!InOwningListDataObject) return;
	
	CachedOwningDataObject = InOwningListDataObject;
	
	if (SettingDisplayNameCommonText)
	{
		SettingDisplayNameCommonText->SetText(InOwningListDataObject->GetDataDisplayName());
	}

	if (!InOwningListDataObject->OnListDataModified.IsBoundToObject(this))
	{
		InOwningListDataObject->OnListDataModified.AddUObject(this, &ThisClass::OnOwningListDataObjectModified);
	}

	if (!InOwningListDataObject->OnDependencyDataModified.IsBoundToObject(this))
	{
		InOwningListDataObject->OnDependencyDataModified.AddUObject(this, &ThisClass::OnOwningDependencyDataObjectModified);
	}

	OnToggleEditableState(InOwningListDataObject->IsDataCurrentlyEditable());
}

void UListEntryWidgetBase::OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EDeltaOptionsListDataModifyReasonType ModifyReason)
{
	
}

void UListEntryWidgetBase::OnOwningDependencyDataObjectModified(UListDataObjectBase* OwningModifiedDependencyData, EDeltaOptionsListDataModifyReasonType ModifyReason)
{
	if (CachedOwningDataObject)
	{
		OnToggleEditableState(CachedOwningDataObject->IsDataCurrentlyEditable());
	}
}

void UListEntryWidgetBase::OnToggleEditableState(bool bIsEditable)
{
	if (SettingDisplayNameCommonText)
	{
		SettingDisplayNameCommonText->SetIsEnabled(bIsEditable); 
	}
}

void UListEntryWidgetBase::SelectThisEntryWidget()
{
	CastChecked<UListView>(GetOwningListView())->SetSelectedItem(GetListItem());
}
