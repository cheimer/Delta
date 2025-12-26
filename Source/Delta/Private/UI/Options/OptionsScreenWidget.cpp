// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Options/OptionsScreenWidget.h"

#include "ICommonInputModule.h"
#include "GameUserSettings/FrontGameUserSettings.h"
#include "Helper/DeltaDebugHelper.h"
#include "Input/CommonUIInputTypes.h"
#include "Subsystem/FrontUISubsystem.h"
#include "UI/Components/FrontCommonButtonBase.h"
#include "UI/Components/FrontCommonListView.h"
#include "UI/Components/FrontTabListWidgetBase.h"
#include "UI/Options/OptionsDataRegistry.h"
#include "UI/Options/OptionsDetailWidget.h"
#include "UI/Options/DataObject/ListDataObjectCollection.h"
#include "UI/Options/ListEntries/ListEntryWidgetBase.h"

void UOptionsScreenWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!ResetAction.IsNull())
	{
		ResetActionHandle = RegisterUIActionBinding(
			FBindUIActionArgs(
				ResetAction,
				true,
				FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionTriggered)
			)
		);
	}

	RegisterUIActionBinding(
		FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultBackAction(),
			true,
			FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered)
		)
	);

	OptionsTabListWidget->OnTabSelected.AddUniqueDynamic(this, &ThisClass::OnOptionsTabSelected);

	OptionsListView->OnItemIsHoveredChanged().AddUObject(this, &ThisClass::OnListViewItemHovered);
	OptionsListView->OnItemSelectionChanged().AddUObject(this, &ThisClass::OnListViewItemSelected);
	
}

void UOptionsScreenWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	for (UListDataObjectCollection* TabCollection : GetOrCreateDataRegistry()->GetRegisteredOptionsTabCollections())
	{
		if (!TabCollection) continue;

		FName TabID = TabCollection->GetDataID();
		if (OptionsTabListWidget->GetTabButtonBaseByID(TabID)) continue;

		OptionsTabListWidget->RequestRegisterTab(TabID, TabCollection->GetDataDisplayName());
		
	}
}

void UOptionsScreenWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	UFrontGameUserSettings::Get()->ApplySettings(true);
}

UWidget* UOptionsScreenWidget::NativeGetDesiredFocusTarget() const
{
	UWidget* SuperValue = Super::NativeGetDesiredFocusTarget();

	if (UObject* SelectedObject = OptionsListView->GetSelectedItem())
	{
		if (UUserWidget* SelectedEntryWidget = OptionsListView->GetEntryWidgetFromItem(SelectedObject))
		{
			return SelectedEntryWidget;
		}
	}

	return SuperValue;
}

UOptionsDataRegistry* UOptionsScreenWidget::GetOrCreateDataRegistry()
{
	if (!OwningDataRegistry)
	{
		OwningDataRegistry = NewObject<UOptionsDataRegistry>();
		OwningDataRegistry->InitOptionsDataRegistry(GetOwningLocalPlayer());
	}

	check(OwningDataRegistry);

	return OwningDataRegistry;
}

void UOptionsScreenWidget::OnResetBoundActionTriggered()
{
	if (ResettableDataArray.IsEmpty()) return;

	UCommonButtonBase* SelectedTabButton = OptionsTabListWidget->GetTabButtonBaseByID(OptionsTabListWidget->GetActiveTab());
	FString SelectedTabButtonName = CastChecked<UFrontCommonButtonBase>(SelectedTabButton)->GetButtonDisplayText().ToString();

	UFrontUISubsystem::Get(GetWorld())->PushConfirmScreenToModalStackAsync(
		EDeltaConfirmType::YesNo,
		FText::FromString(TEXT("Reset")),
		FText::FromString(TEXT("Sure to reset ") + SelectedTabButtonName + TEXT(" settings?")),
		[this](EDeltaConfirmButtonType ClickedButtonType)
		{
			if (ClickedButtonType != EDeltaConfirmButtonType::Confirmed) return;

			bIsResettingData = true;
			bool bHasDataFailedToReset = false;
			
			for (UListDataObjectBase* DataToReset : ResettableDataArray)
			{
				if (!DataToReset) continue;

				if (DataToReset->TryResetBackToDefaultValue())
				{
					DeltaDebug::Print(TEXT("Reset option"));
				}
				else
				{
					bHasDataFailedToReset = true;
				}
			}

			if (!bHasDataFailedToReset)
			{
				ResettableDataArray.Empty();
				RemoveActionBinding(ResetActionHandle);
			}
			
			bIsResettingData = false;
		}
	);
	
}

void UOptionsScreenWidget::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}

void UOptionsScreenWidget::OnOptionsTabSelected(FName TabID)
{
	ListEntryInfoDetailsView->ClearDetailsViewInfo();
	
	TArray<UListDataObjectBase*> FoundListSourceItems = GetOrCreateDataRegistry()->GetListSourceItemsBySelectedTabID(TabID);

	OptionsListView->SetListItems(FoundListSourceItems);
	OptionsListView->RequestRefresh();

	if (OptionsListView->GetNumItems() != 0)
	{
		OptionsListView->NavigateToIndex(0);
		OptionsListView->SetSelectedIndex(0);
	}

	ResettableDataArray.Empty();

	for (UListDataObjectBase* FoundListSourceItem : FoundListSourceItems)
	{
		if (!FoundListSourceItem) continue;

		if (!FoundListSourceItem->OnListDataModified.IsBoundToObject(this))
		{
			FoundListSourceItem->OnListDataModified.AddUObject(this, &ThisClass::OnListViewListDataModified);
		}

		if (FoundListSourceItem->CanResetBackToDefaultValue())
		{
			ResettableDataArray.AddUnique(FoundListSourceItem);
		}
	}

	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
	else
	{
		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
	
}

void UOptionsScreenWidget::OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered)
{
	if (!InHoveredItem) return;

	UListEntryWidgetBase* HoveredEntryWidget = OptionsListView->GetEntryWidgetFromItem<UListEntryWidgetBase>(InHoveredItem);
	check(HoveredEntryWidget);

	HoveredEntryWidget->NativeOnListEntryWidgetHovered(bWasHovered);

	if (bWasHovered)
	{
		ListEntryInfoDetailsView->UpdateDetailsViewInfo(CastChecked<UListDataObjectBase>(InHoveredItem), TryGetEntryWidgetClassName(InHoveredItem));
	}
	else
	{
		if (UListDataObjectBase* SelectedItem = OptionsListView->GetSelectedItem<UListDataObjectBase>())
		{
			ListEntryInfoDetailsView->UpdateDetailsViewInfo(SelectedItem, TryGetEntryWidgetClassName(SelectedItem));
		}
	}
}

void UOptionsScreenWidget::OnListViewItemSelected(UObject* InSelectedItem)
{
	if (!InSelectedItem) return;
	
	ListEntryInfoDetailsView->UpdateDetailsViewInfo(CastChecked<UListDataObjectBase>(InSelectedItem), TryGetEntryWidgetClassName(InSelectedItem));
}

FString UOptionsScreenWidget::TryGetEntryWidgetClassName(UObject* InOwningListItem) const
{
	if (UUserWidget* FoundEntryWidget = OptionsListView->GetEntryWidgetFromItem(InOwningListItem))
	{
		return FoundEntryWidget->GetClass()->GetName();
	}

	return TEXT("Not Valid");
}

void UOptionsScreenWidget::OnListViewListDataModified(UListDataObjectBase* ModifiedData, EDeltaOptionsListDataModifyReasonType ModifiedReason)
{
	if (!ModifiedData || bIsResettingData) return;

	if (ModifiedData->CanResetBackToDefaultValue())
	{
		ResettableDataArray.AddUnique(ModifiedData);

		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
	else
	{
		if (ResettableDataArray.Contains(ModifiedData))
		{
			ResettableDataArray.Remove(ModifiedData);
		}
	}

	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
}
