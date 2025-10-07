// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Options/ListEntries/ListEntryWidgetString.h"

#include "CommonInputSubsystem.h"
#include "Helper/DeltaDebugHelper.h"
#include "UI/Components/FrontCommonButtonBase.h"
#include "UI/Components/FrontCommonRotator.h"
#include "UI/Options/DataObject/ListDataObjectBase.h"
#include "UI/Options/DataObject/ListDataObjectString.h"

void UListEntryWidgetString::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BeforeOptionCommonButton->OnClicked().AddUObject(this, &ThisClass::OnBeforeOptionButtonClicked);
	
	NextOptionCommonButton->OnClicked().AddUObject(this, &ThisClass::OnNextOptionButtonClicked);

	AvailableOptionsCommonRotator->OnClicked().AddLambda([this]()
	{
		SelectThisEntryWidget();
	});
	AvailableOptionsCommonRotator->OnRotatedEvent.AddUObject(this, &ThisClass::OnRotatorValueChanged);
}

void UListEntryWidgetString::OnOwningListDataObjectSet(UListDataObjectBase* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningStringDataObject = CastChecked<UListDataObjectString>(InOwningListDataObject);

	AvailableOptionsCommonRotator->PopulateTextLabels(CachedOwningStringDataObject->GetAvailableOptionsTextArray());
	AvailableOptionsCommonRotator->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
	
}

void UListEntryWidgetString::OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EDeltaOptionsListDataModifyReasonType ModifyReason)
{
	Super::OnOwningListDataObjectModified(OwningModifiedData, ModifyReason);

	if (CachedOwningStringDataObject)
	{
		AvailableOptionsCommonRotator->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
	}
	
}

void UListEntryWidgetString::OnToggleEditableState(bool bIsEditable)
{
	Super::OnToggleEditableState(bIsEditable);

	BeforeOptionCommonButton->SetIsEnabled(bIsEditable);
	NextOptionCommonButton->SetIsEnabled(bIsEditable);
	AvailableOptionsCommonRotator->SetIsEnabled(bIsEditable);
}

void UListEntryWidgetString::OnBeforeOptionButtonClicked()
{
	if (CachedOwningStringDataObject)
	{
		CachedOwningStringDataObject->BackToBeforeOption();
	}

	SelectThisEntryWidget();
}

void UListEntryWidgetString::OnNextOptionButtonClicked()
{
	if (CachedOwningStringDataObject)
	{
		CachedOwningStringDataObject->AdvanceToNextOption();
	}
	
	SelectThisEntryWidget();
}

void UListEntryWidgetString::OnRotatorValueChanged(int32 Value, bool bUserInitiated)
{
	if (!CachedOwningStringDataObject || !bUserInitiated) return;

	UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();
	if (!CommonInputSubsystem) return;

	if (CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		CachedOwningStringDataObject->OnRotatorInitiatedValueChanged(AvailableOptionsCommonRotator->GetSelectedText());
	}
	
}
