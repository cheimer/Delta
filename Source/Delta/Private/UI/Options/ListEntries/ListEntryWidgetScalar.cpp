// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Options/ListEntries/ListEntryWidgetScalar.h"

#include "AnalogSlider.h"
#include "UI/Options/DataObject/ListDataObjectScalar.h"

void UListEntryWidgetScalar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SettingAnalogSlider->OnValueChanged.AddUniqueDynamic(this, &ThisClass::OnSliderValueChanged);
	SettingAnalogSlider->OnMouseCaptureBegin.AddUniqueDynamic(this, &ThisClass::OnSliderMouseCaptureBegin);
	
}

void UListEntryWidgetScalar::OnOwningListDataObjectSet(UListDataObjectBase* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningScalarDataObject = CastChecked<UListDataObjectScalar>(InOwningListDataObject);

	SettingValueCommonNumericText->SetNumericType(CachedOwningScalarDataObject->GetDisplayNumericType());
	SettingValueCommonNumericText->FormattingSpecification = CachedOwningScalarDataObject->GetNumberFormattingOptions();
	SettingValueCommonNumericText->SetCurrentValue(CachedOwningScalarDataObject->GetCurrentValue());

	SettingAnalogSlider->SetMinValue(CachedOwningScalarDataObject->GetDisplayValueRange().GetLowerBoundValue());
	SettingAnalogSlider->SetMaxValue(CachedOwningScalarDataObject->GetDisplayValueRange().GetUpperBoundValue());
	SettingAnalogSlider->SetStepSize(CachedOwningScalarDataObject->GetSliderStepSize());
	SettingAnalogSlider->SetValue(CachedOwningScalarDataObject->GetCurrentValue());
	
}

void UListEntryWidgetScalar::OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EDeltaOptionsListDataModifyReasonType ModifyReason)
{
	Super::OnOwningListDataObjectModified(OwningModifiedData, ModifyReason);

	if (CachedOwningScalarDataObject)
	{
		SettingValueCommonNumericText->SetCurrentValue(CachedOwningScalarDataObject->GetCurrentValue());
		SettingAnalogSlider->SetValue(CachedOwningScalarDataObject->GetCurrentValue());
	}
	
}

void UListEntryWidgetScalar::OnSliderValueChanged(float Value)
{
	if (CachedOwningScalarDataObject)
	{
		CachedOwningScalarDataObject->SetCurrentValueFromSlider(Value);
	}
}

void UListEntryWidgetScalar::OnSliderMouseCaptureBegin()
{
	SelectThisEntryWidget();
}
