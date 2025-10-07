// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Options/DataObject/ListDataObjectScalar.h"

#include "UI/Options/OptionsDataInteractionHelper.h"

FCommonNumberFormattingOptions UListDataObjectScalar::NoDecimal()
{
	FCommonNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = 0;

	return Options;
}

FCommonNumberFormattingOptions UListDataObjectScalar::WithDecimal(int32 NumFracDigit)
{
	FCommonNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = NumFracDigit;

	return Options;
}

float UListDataObjectScalar::GetCurrentValue() const
{
	if (DataDynamicGetter)
	{
		return FMath::GetMappedRangeValueClamped(
			OutputValueRange,
			DisplayValueRange,
			StringToFloat(DataDynamicGetter->GetValueAsString())
		);
	}

	return 0.0f;
}

void UListDataObjectScalar::SetCurrentValueFromSlider(float InNewValue)
{
	if (DataDynamicSetter)
	{
		const float ClampedValue = FMath::GetMappedRangeValueClamped(
			DisplayValueRange,
			OutputValueRange,
			InNewValue
		);
		DataDynamicSetter->SetValueFromString(FString::SanitizeFloat(ClampedValue));

		NotifyListDataModified(this);
	}
}

bool UListDataObjectScalar::CanResetBackToDefaultValue() const
{
	bool SuperValue = Super::CanResetBackToDefaultValue();

	if (HasDefaultValue() && DataDynamicGetter)
	{
		const float DefaultValue = StringToFloat(GetDefaultValueAsString());
		const float CurrentValue = StringToFloat(DataDynamicGetter->GetValueAsString());

		if (FMath::IsNearlyEqual(DefaultValue, CurrentValue, 0.01f))
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	return SuperValue;
}

bool UListDataObjectScalar::TryResetBackToDefaultValue()
{
	bool SuperValue = Super::TryResetBackToDefaultValue();

	if (CanResetBackToDefaultValue() && DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(GetDefaultValueAsString());

		NotifyListDataModified(this, EDeltaOptionsListDataModifyReasonType::ResetToDefault);

		return true;
	}

	return SuperValue;
}

void UListDataObjectScalar::OnEditDependencyDataModified(UListDataObjectBase* ModifiedDependencyData, EDeltaOptionsListDataModifyReasonType ModifyReason)
{
	NotifyListDataModified(this, EDeltaOptionsListDataModifyReasonType::DependencyModified);
	
	Super::OnEditDependencyDataModified(ModifiedDependencyData, ModifyReason);

}

float UListDataObjectScalar::StringToFloat(const FString& InString) const
{
	float ConvertedFloat = 0.0f;
	LexFromString(ConvertedFloat, *InString);

	return ConvertedFloat;
}
